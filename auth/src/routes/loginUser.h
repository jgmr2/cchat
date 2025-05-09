#pragma once
#include "crow.h"
#include "../db.h"
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/database.hpp>
#include <mongocxx/collection.hpp>
#include <bsoncxx/exception/exception.hpp>
#include <bcrypt/BCrypt.hpp>
#include <jwt-cpp/jwt.h> // Biblioteca para manejar JWT

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

namespace routes {
    void setup_login(crow::App<crow::CORSHandler>& app, MongoDBConnection& db_connection, const std::string& jwt_secret) {
        // Manejar solicitudes OPTIONS (preflight)
        CROW_ROUTE(app, "/login").methods(crow::HTTPMethod::OPTIONS)([](const crow::request& req) {
            crow::response res;
            res.code = 204; // Sin contenido
            return res;
        });

        // Manejar solicitudes POST para login
        CROW_ROUTE(app, "/login").methods(crow::HTTPMethod::Post)([&db_connection, &jwt_secret](const crow::request& req) {
            try {
                // Verificamos que haya cuerpo en la solicitud
                if (req.body.empty()) {
                    return crow::response(400, R"({"error": "Invalid request: Body is empty"})");
                }

                // Parseamos el body de la petición como BSON
                auto body_doc = bsoncxx::from_json(req.body);
                auto doc_view = body_doc.view();

                // Validamos y extraemos los campos
                auto email_elem = doc_view["email"];
                auto password_elem = doc_view["password"];

                if (!email_elem || !password_elem) {
                    return crow::response(400, R"({"error": "Missing required fields: email or password"})");
                }

                auto email = std::string(email_elem.get_string().value);
                auto password = std::string(password_elem.get_string().value);

                // Verificamos que los campos no estén vacíos
                if (email.empty() || password.empty()) {
                    return crow::response(400, R"({"error": "Email or password cannot be empty"})");
                }

                // Accedemos a la colección "users"
                auto db = db_connection.get_database();
                mongocxx::collection users = db["users"];

                // Buscamos al usuario por correo electrónico
                auto user_doc = users.find_one(make_document(kvp("email", email)));
                if (!user_doc) {
                    return crow::response(401, R"({"error": "Invalid email or password"})");
                }

                // Extraemos la contraseña hasheada del documento
                auto user_view = user_doc->view();
                auto hashed_password_elem = user_view["password"];
                if (!hashed_password_elem) {
                    return crow::response(500, R"({"error": "Password not found for user"})");
                }

                auto hashed_password = std::string(hashed_password_elem.get_string().value);

                // Verificamos la contraseña
                if (!BCrypt::validatePassword(password, hashed_password)) {
                    return crow::response(401, R"({"error": "Invalid email or password"})");
                }

                // Generamos el token JWT
                auto token = jwt::create()
                                 .set_issuer("auth_service")
                                 .set_type("JWT")
                                 .set_payload_claim("email", jwt::claim(email))
                                 .set_issued_at(std::chrono::system_clock::now())
                                 .set_expires_at(std::chrono::system_clock::now() + std::chrono::hours(24)) // Expira en 24 horas
                                 .sign(jwt::algorithm::hs256{jwt_secret});

                // Devolvemos el token al cliente
                crow::json::wvalue response;
                response["message"] = "Login successful";
                response["token"] = token;
                return crow::response(200, response);
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