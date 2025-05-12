#pragma once

#include <crow.h>
#include <crow/middlewares/cors.h>
#include "../db.h"
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/exception/exception.hpp>
#include <mongocxx/collection.hpp>
#include <mongocxx/cursor.hpp>
#include "../json.hpp" // Archivo único de nlohmann/json
#include <jwt-cpp/jwt.h> // Biblioteca para manejar JWT
#include <vector>
#include <string>
#include <iostream>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;
using json = nlohmann::json;

inline void setup_models_routes(crow::App<crow::CORSHandler>& app, MongoDBConnection& db_connection, const std::string& jwt_secret) {
    auto& database = db_connection.get_database();
    auto uploads_collection = database["uploads"]; // Acceso a la colección "uploads"

    std::cout << "[INFO] Configurando rutas de modelos..." << std::endl;

    CROW_ROUTE(app, "/uploads/search").methods(crow::HTTPMethod::GET)(
    [&uploads_collection, jwt_secret](const crow::request& req) -> crow::response {
        std::cout << "[DEBUG] Iniciando procesamiento de solicitud GET /uploads/search" << std::endl;

        // Validar el token JWT
        auto auth_header = req.get_header_value("Authorization");
        if (auth_header.empty() || auth_header.find("Bearer ") != 0) {
            std::cerr << "[ERROR] Falta el encabezado Authorization o no es válido." << std::endl;
            return crow::response(401, R"({"error":"Unauthorized"})");
        }

        std::string token = auth_header.substr(7); // Eliminar "Bearer " del encabezado
        try {
            auto decoded = jwt::decode(token);
            auto verifier = jwt::verify()
                                .allow_algorithm(jwt::algorithm::hs256{jwt_secret})
                                .with_issuer("auth_service");
            verifier.verify(decoded);

            std::cout << "[INFO] Token JWT válido." << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "[ERROR] Token JWT inválido: " << e.what() << std::endl;
            return crow::response(401, R"({"error":"Unauthorized"})");
        }

        // Obtener el parámetro de búsqueda
        auto query_param = req.url_params.get("q");
        if (!query_param) {
            std::cerr << "[ERROR] Parámetro de búsqueda 'q' ausente" << std::endl;
            return crow::response(400, R"({"error":"Missing search parameter 'q'"})");
        }

        std::string search_query = query_param;
        std::cout << "[DEBUG] Parámetro de búsqueda recibido: " << search_query << std::endl;

        try {
            // Construir el filtro de búsqueda para MongoDB
            auto filter = make_document(
                kvp("$or", [&search_query](bsoncxx::builder::basic::sub_array array) {
                    array.append(
                        make_document(kvp("description", make_document(kvp("$regex", search_query), kvp("$options", "i"))))
                    );
                    array.append(
                        make_document(kvp("tags", make_document(kvp("$regex", search_query), kvp("$options", "i"))))
                    );
                })
            );

            // Realizar la consulta a la colección
            std::cout << "[DEBUG] Realizando consulta a la colección 'uploads' con filtro: " << bsoncxx::to_json(filter) << std::endl;
            mongocxx::cursor cursor = uploads_collection.find(filter.view());
            std::cout << "[DEBUG] Consulta a la colección 'uploads' completada" << std::endl;

            // Verificar si el cursor contiene documentos
            if (cursor.begin() == cursor.end()) {
                std::cout << "[DEBUG] No se encontraron coincidencias para la búsqueda" << std::endl;
                return crow::response(200, R"({"uploads":[]})");
            }

            // Construir la respuesta JSON con los IDs de los documentos coincidentes
            json response;
            response["uploads"] = json::array();

            std::cout << "[DEBUG] Procesando documentos obtenidos de la colección" << std::endl;
            for (const auto& doc : cursor) {
                try {
                    // Extraer el ObjectId del documento
                    if (doc["_id"]) {
                        bsoncxx::oid id = doc["_id"].get_oid().value;
                        response["uploads"].push_back(id.to_string());
                    } else {
                        std::cerr << "[ERROR] Documento sin campo '_id'" << std::endl;
                    }
                } catch (const bsoncxx::exception& e) {
                    std::cerr << "[ERROR] Error al procesar un documento BSON: " << e.what() << std::endl;
                } catch (const std::exception& e) {
                    std::cerr << "[ERROR] Error inesperado al procesar un documento: " << e.what() << std::endl;
                }
            }

            std::cout << "[DEBUG] Todos los documentos procesados correctamente" << std::endl;

            // Log para mostrar la respuesta completa antes de enviarla
            std::cout << "[DEBUG] Respuesta JSON construida: " << response.dump() << std::endl;

            std::cout << "[DEBUG] Respuesta construida correctamente, enviando respuesta al cliente" << std::endl;
            return crow::response(200, response.dump());
        } catch (const mongocxx::exception& e) {
            std::cerr << "[ERROR] Error al consultar la base de datos: " << e.what() << std::endl;

            // Construir un objeto JSON para el error
            json error_response;
            error_response["error"] = "Error querying the database.";
            error_response["details"] = e.what();

            std::cout << "[DEBUG] Enviando respuesta de error al cliente" << std::endl;
            return crow::response(500, error_response.dump());
        } catch (const std::exception& e) {
            std::cerr << "[ERROR] Error inesperado: " << e.what() << std::endl;

            // Construir un objeto JSON para el error
            json error_response;
            error_response["error"] = "Unexpected error.";
            error_response["details"] = e.what();

            std::cout << "[DEBUG] Enviando respuesta de error al cliente" << std::endl;
            return crow::response(500, error_response.dump());
        }
    });

    std::cout << "[INFO] Rutas de modelos configuradas correctamente." << std::endl;
}