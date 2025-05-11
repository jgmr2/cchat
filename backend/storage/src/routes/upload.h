#pragma once

#include <fstream>
#include <filesystem>
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>
#include <iostream> // Para logs
#include "../db.h"
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/types.hpp>
#include <jwt-cpp/jwt.h> // Biblioteca para manejar JWT

// Función para calcular el hash SHA-512
inline std::string calculate_sha512(const std::string& data) {
    unsigned char hash[SHA512_DIGEST_LENGTH];
    SHA512(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), hash);

    std::ostringstream oss;
    for (int i = 0; i < SHA512_DIGEST_LENGTH; ++i) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return oss.str();
}

// Función para obtener la extensión del archivo
inline std::string get_file_extension(const std::string& filename) {
    size_t dot_pos = filename.find_last_of('.');
    if (dot_pos != std::string::npos) {
        return filename.substr(dot_pos);
    }
    return "";
}

// Implementación de la función para configurar las rutas
inline void setup_upload_routes(crow::App<crow::CORSHandler>& app, MongoDBConnection& db_connection, const std::string& jwt_secret) {
    auto& database = db_connection.get_database();

    std::cout << "[INFO] Configurando rutas de subida de archivos..." << std::endl;

    // Ruta para manejar solicitudes OPTIONS (preflight)
    CROW_ROUTE(app, "/upload").methods(crow::HTTPMethod::OPTIONS)([](const crow::request& req) {
        std::cout << "[INFO] Solicitud OPTIONS recibida en /upload" << std::endl;
        crow::response res;
        res.code = 204; // No Content
        return res;
    });

    // Ruta para subir un archivo
    CROW_ROUTE(app, "/upload").methods(crow::HTTPMethod::POST)(
        [&database, &jwt_secret](const crow::request& req) {
            std::cout << "[INFO] Solicitud POST recibida en /upload" << std::endl;

            // Validar el token JWT
            std::string auth_header = req.get_header_value("Authorization");
            if (auth_header.empty() || auth_header.find("Bearer ") != 0) {
                std::cerr << "[ERROR] Falta el encabezado Authorization o no es válido." << std::endl;
                return crow::response(401, "Missing or invalid Authorization header.");
            }

            std::string token = auth_header.substr(7); // Eliminar "Bearer " del encabezado
            try {
                auto decoded = jwt::decode(token);
                auto verifier = jwt::verify()
                                    .allow_algorithm(jwt::algorithm::hs256{jwt_secret})
                                    .with_issuer("auth_service"); // Cambia "auth0" por tu emisor si es necesario
                verifier.verify(decoded);
                std::cout << "[INFO] Token JWT válido." << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "[ERROR] Token JWT inválido: " << e.what() << std::endl;
                return crow::response(401, "Invalid token.");
            }

            const std::string upload_dir = "/mnt/cdb/";

            if (req.body.empty()) {
                std::cerr << "[ERROR] El cuerpo de la solicitud está vacío." << std::endl;
                return crow::response(400, "No file provided in the request body.");
            }

            std::string original_filename = req.get_header_value("X-Filename");
            if (original_filename.empty()) {
                std::cerr << "[ERROR] Falta el encabezado X-Filename." << std::endl;
                return crow::response(400, "Missing X-Filename header.");
            }

            std::cout << "[INFO] Nombre del archivo recibido: " << original_filename << std::endl;

            std::string file_extension = get_file_extension(original_filename);
            std::string sha512_hash = calculate_sha512(req.body);
            const std::string file_path = upload_dir + sha512_hash + file_extension;

            std::cout << "[INFO] Hash SHA-512 calculado: " << sha512_hash << std::endl;
            std::cout << "[INFO] Ruta completa del archivo: " << file_path << std::endl;

            try {
                std::ofstream file(file_path, std::ios::binary);
                if (!file) {
                    std::cerr << "[ERROR] No se pudo abrir el archivo para escritura: " << file_path << std::endl;
                    return crow::response(500, "Failed to open file for writing.");
                }
                file.write(req.body.c_str(), req.body.size());
                file.close();
                std::cout << "[INFO] Archivo guardado exitosamente en: " << file_path << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "[ERROR] Error al guardar el archivo: " << e.what() << std::endl;
                return crow::response(500, std::string("Error saving file: ") + e.what());
            }

            try {
                auto collection = database["uploads"];
                bsoncxx::builder::stream::document document{};
                document << "filename" << original_filename
                         << "hash" << sha512_hash
                         << "path" << file_path
                         << "timestamp" << bsoncxx::types::b_date(std::chrono::system_clock::now());
                collection.insert_one(document.view());
                std::cout << "[INFO] Metadatos del archivo guardados en la base de datos." << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "[ERROR] Error al guardar metadatos en la base de datos: " << e.what() << std::endl;
                return crow::response(500, std::string("Error saving metadata to database: ") + e.what());
            }

            return crow::response(200, "File uploaded successfully with SHA-512 hash: " + sha512_hash + file_extension);
        });

    std::cout << "[INFO] Rutas de subida de archivos configuradas correctamente." << std::endl;
}