#pragma once
#include "crow.h"
#include "../db.h"
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/database.hpp>
#include <mongocxx/collection.hpp>
#include <mongocxx/result/insert_one.hpp>
#include <bsoncxx/exception/exception.hpp>
#include <bcrypt/BCrypt.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

namespace routes {
    void setup_register(crow::App<crow::CORSHandler>& app, MongoDBConnection& db_connection, const std::string& jwt_secret) {
        // Manejar solicitudes OPTIONS (preflight)
        CROW_ROUTE(app, "/register").methods(crow::HTTPMethod::OPTIONS)([](const crow::request& req) {
            crow::response res;
            res.code = 204; // Sin contenido
            return res;
        });

        // Manejar solicitudes POST para registro
        CROW_ROUTE(app, "/register").methods(crow::HTTPMethod::Post)([&db_connection](const crow::request& req) {
            try {
                // Verificamos que haya cuerpo en la solicitud
                if (req.body.empty()) {
                    return crow::response(400, R"({"error": "Invalid request: Body is empty"})");
                }

                // Parseamos el body de la petición como BSON
                auto body_doc = bsoncxx::from_json(req.body);
                auto doc_view = body_doc.view();

                // Validamos y extraemos los campos
                auto username_elem = doc_view["username"];
                auto email_elem = doc_view["email"];
                auto password_elem = doc_view["password"];

                if (!username_elem || !email_elem || !password_elem) {
                    return crow::response(400, R"({"error": "Missing required fields: username, email, or password"})");
                }

                auto username = std::string(username_elem.get_string().value);
                auto email = std::string(email_elem.get_string().value);
                auto password = std::string(password_elem.get_string().value);

                // Verificamos que los campos no estén vacíos
                if (username.empty() || email.empty() || password.empty()) {
                    return crow::response(400, R"({"error": "Fields cannot be empty"})");
                }

                // Hashear la contraseña
                std::string hashed_password = BCrypt::generateHash(password);

                // Accedemos a la colección "users"
                auto db = db_connection.get_database();
                mongocxx::collection users = db["users"];

                // Verificamos si el usuario ya existe
                auto existing_user = users.find_one(make_document(kvp("email", email)));
                if (existing_user) {
                    return crow::response(409, R"({"error": "User already exists"})");
                }

                // Insertamos el nuevo usuario
                auto result = users.insert_one(
                    make_document(
                        kvp("username", username),
                        kvp("email", email),
                        kvp("password", hashed_password) // ¡IMPORTANTE! Hashea la contraseña antes de guardar
                    )
                );

                // Verificamos si la inserción fue exitosa
                if (result && result->result().inserted_count() == 1) {
                    return crow::response(201, R"({"message": "User registered successfully"})");
                } else {
                    return crow::response(500, R"({"error": "Failed to register user"})");
                }
            } catch (const bsoncxx::exception& e) {
                // Manejo de errores de BSON
                return crow::response(400, "{\"error\": \"Failed to parse JSON body: " + std::string(e.what()) + "\"}");
            } catch (const std::exception& e) {
                // Manejo de otros errores
                return crow::response(500, "{\"error\": \"" + std::string(e.what()) + "\"}");
            }
        });
    }
}