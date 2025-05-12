#pragma once

#include <fstream>
#include <filesystem>
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>
#include <iostream> // Para logs
#include "../db.h"
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/array.hpp>
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
            std::string user_id;
            try {
                auto decoded = jwt::decode(token);
                auto verifier = jwt::verify()
                                    .allow_algorithm(jwt::algorithm::hs256{jwt_secret})
                                    .with_issuer("auth_service");
                verifier.verify(decoded);

                // Extraer el ID del usuario del token JWT
                if (decoded.has_payload_claim("user_id")) {
                    user_id = decoded.get_payload_claim("user_id").as_string();
                    std::cout << "[INFO] Token JWT válido. ID de usuario: " << user_id << std::endl;
                } else {
                    std::cerr << "[ERROR] El token JWT no contiene el ID del usuario (claim 'user_id')." << std::endl;
                    return crow::response(401, "Invalid token: missing user ID.");
                }
            } catch (const std::exception& e) {
                std::cerr << "[ERROR] Token JWT inválido: " << e.what() << std::endl;
                return crow::response(401, "Invalid token.");
            }

            const std::string upload_dir = "/mnt/cdb/";

            if (req.body.empty()) {
                std::cerr << "[ERROR] El cuerpo de la solicitud está vacío." << std::endl;
                return crow::response(400, "No file provided in the request body.");
            }

            // Obtener los encabezados adicionales
            std::string original_filename = req.get_header_value("X-Filename");
            if (original_filename.empty()) {
                std::cerr << "[ERROR] Falta el encabezado X-Filename." << std::endl;
                return crow::response(400, "Missing X-Filename header.");
            }

            std::string description = req.get_header_value("X-Description");
            if (description.empty()) {
                std::cerr << "[ERROR] Falta el encabezado X-Description." << std::endl;
                return crow::response(400, "Missing X-Description header.");
            }

            std::string tags_json = req.get_header_value("X-Tags");
            if (tags_json.empty()) {
                std::cerr << "[ERROR] Falta el encabezado X-Tags." << std::endl;
                return crow::response(400, "Missing X-Tags header.");
            }

            std::cout << "[INFO] Nombre del archivo recibido: " << original_filename << std::endl;
            std::cout << "[INFO] Descripción recibida: " << description << std::endl;
            std::cout << "[INFO] Etiquetas recibidas: " << tags_json << std::endl;

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
                // Guardar los metadatos del archivo en la base de datos
                auto collection = database["uploads"];
                bsoncxx::builder::basic::document document{};
                bsoncxx::builder::basic::array tags_array;

                // Construir manualmente el array BSON para las etiquetas
                size_t start = 0, end = 0;
                while ((end = tags_json.find(',', start)) != std::string::npos) {
                    std::string tag = tags_json.substr(start, end - start);
                    tag.erase(std::remove(tag.begin(), tag.end(), '\"'), tag.end());
                    tag.erase(std::remove(tag.begin(), tag.end(), '['), tag.end());
                    tag.erase(std::remove(tag.begin(), tag.end(), ']'), tag.end());
                    tag.erase(std::remove(tag.begin(), tag.end(), ' '), tag.end());
                    if (!tag.empty()) {
                        tags_array.append(tag);
                    }
                    start = end + 1;
                }
                std::string last_tag = tags_json.substr(start);
                last_tag.erase(std::remove(last_tag.begin(), last_tag.end(), '\"'), last_tag.end());
                last_tag.erase(std::remove(last_tag.begin(), last_tag.end(), '['), last_tag.end());
                last_tag.erase(std::remove(last_tag.begin(), last_tag.end(), ']'), last_tag.end());
                last_tag.erase(std::remove(last_tag.begin(), last_tag.end(), ' '), last_tag.end());
                if (!last_tag.empty()) {
                    tags_array.append(last_tag);
                }

                document.append(
                    bsoncxx::builder::basic::kvp("user_id", user_id),
                    bsoncxx::builder::basic::kvp("filename", original_filename),
                    bsoncxx::builder::basic::kvp("description", description),
                    bsoncxx::builder::basic::kvp("tags", tags_array),
                    bsoncxx::builder::basic::kvp("hash", sha512_hash),
                    bsoncxx::builder::basic::kvp("path", file_path),
                    bsoncxx::builder::basic::kvp("timestamp", bsoncxx::types::b_date(std::chrono::system_clock::now()))
                );

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