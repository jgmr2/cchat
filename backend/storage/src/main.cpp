#include "crow.h"
#include "crow/middlewares/cors.h" // Incluir el middleware CORSHandler
#include <fstream>
#include <filesystem>
#include <unordered_map>
#include <openssl/sha.h> // Biblioteca para calcular SHA-512
#include <regex> // Para validar el formato del token
#include <sstream> // Para manejar cadenas y conversiones

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

// Función para validar el token (puedes personalizarla según tus necesidades)
bool validate_token(const std::string& token) {
    if (token.empty()) {
        return false;
    }

    // Eliminar el prefijo "Bearer " si está presente
    std::string actual_token = token;
    const std::string prefix = "Bearer ";
    if (token.find(prefix) == 0) {
        actual_token = token.substr(prefix.size());
    }

    // Validar el formato del token
    std::regex token_regex("^[A-Za-z0-9\\-_.]+$");
    return std::regex_match(actual_token, token_regex);
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
        .headers("Content-Type", "X-Filename", "Authorization") // Encabezados permitidos
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

        // Paso 1: Validar el token
        std::string token = req.get_header_value("Authorization");
        if (token.empty()) {
            return crow::response(401, "Unauthorized: Missing token.");
        }

        if (!validate_token(token)) {
            return crow::response(401, "Unauthorized: Invalid token.");
        }

        // Paso 2: Validar el encabezado X-Filename
        std::string original_filename = req.get_header_value("X-Filename");
        if (original_filename.empty()) {
            return crow::response(400, "Missing X-Filename header.");
        }

        // Paso 3: Validar el cuerpo de la solicitud
        if (req.body.empty()) {
            return crow::response(400, "No file provided in the request body.");
        }

        // Paso 4: Procesar el archivo
        std::string file_extension = get_file_extension(original_filename);
        std::string sha512_hash = calculate_sha512(req.body);
        const std::string file_path = upload_dir + sha512_hash + file_extension;

        // Crear el directorio si no existe
        try {
            std::filesystem::create_directories(upload_dir);
        } catch (const std::filesystem::filesystem_error& e) {
            return crow::response(500, "Failed to create upload directory: " + std::string(e.what()));
        }

        // Guardar el archivo en el disco
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