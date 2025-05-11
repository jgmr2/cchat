#include "crow.h"
#include "crow/middlewares/cors.h" // Incluir el middleware CORSHandler
#include <fstream>
#include <filesystem>
#include <unordered_map>
#include <openssl/sha.h> // Biblioteca para calcular SHA-512

// Función para calcular el hash SHA-512
std::string calculate_sha512(const std::string& data) {
    unsigned char hash[SHA512_DIGEST_LENGTH];
    SHA512(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), hash);

    // Convertir el hash a una cadena hexadecimal
    std::ostringstream oss;
    for (int i = 0; i < SHA512_DIGEST_LENGTH; ++i) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return oss.str();
}

// Función para obtener la extensión del archivo
std::string get_file_extension(const std::string& filename) {
    size_t dot_pos = filename.find_last_of('.');
    if (dot_pos != std::string::npos) {
        return filename.substr(dot_pos); // Devuelve la extensión, incluyendo el punto
    }
    return ""; // Si no hay extensión, devuelve una cadena vacía
}

int main()
{
    // Inicializar la aplicación con el middleware CORSHandler
    crow::App<crow::CORSHandler> app;

    // Configurar las reglas de CORS globales
    auto& cors = app.get_middleware<crow::CORSHandler>();
    cors.global()
        .origin("*") // Permitir todas las solicitudes de cualquier origen
        .methods(crow::HTTPMethod::GET, crow::HTTPMethod::POST, crow::HTTPMethod::OPTIONS) // Métodos permitidos
        .headers("Content-Type", "X-Filename") // Encabezados permitidos
        .max_age(3600); // Tiempo de caché para preflight

    // Ruta para manejar solicitudes OPTIONS (preflight)
    CROW_ROUTE(app, "/upload").methods(crow::HTTPMethod::OPTIONS)([](const crow::request& req){
        crow::response res;
        res.code = 204; // No Content
        return res;
    });

    // Ruta para subir un archivo
    CROW_ROUTE(app, "/upload").methods(crow::HTTPMethod::POST)([](const crow::request& req){
        // Directorio donde se guardarán los archivos
        const std::string upload_dir = "/mnt/cdb/";

        // Verificar si el cuerpo de la solicitud contiene datos
        if (req.body.empty()) {
            return crow::response(400, "No file provided in the request body.");
        }

        // Obtener el nombre del archivo desde el encabezado X-Filename
        std::string original_filename = req.get_header_value("X-Filename");
        if (original_filename.empty()) {
            return crow::response(400, "Missing X-Filename header.");
        }

        // Obtener la extensión del archivo
        std::string file_extension = get_file_extension(original_filename);

        // Calcular el hash SHA-512 del contenido del archivo
        std::string sha512_hash = calculate_sha512(req.body);

        // Ruta completa del archivo usando el hash como nombre y conservando la extensión
        const std::string file_path = upload_dir + sha512_hash + file_extension;

        // Guardar el archivo en el volumen montado
        try {
            std::ofstream file(file_path, std::ios::binary);
            if (!file) {
                return crow::response(500, "Failed to open file for writing.");
            }
            file.write(req.body.c_str(), req.body.size());
            file.close();
        } catch (const std::exception& e) {
            return crow::response(500, std::string("Error saving file: ") + e.what());
        }

        return crow::response(200, "File uploaded successfully with SHA-512 hash: " + sha512_hash + file_extension);
    });

    // Iniciar el servidor
    app.port(8080).run();
}