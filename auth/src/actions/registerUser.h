#include <string>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/database.hpp>
#include <mongocxx/collection.hpp>
#include <mongocxx/result/insert_one.hpp>
#include <bsoncxx/exception/exception.hpp>
#include "httplib.h"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

std::string registerUser(const httplib::Request& req, mongocxx::database& db) {
    try {
        // Verificamos que sea un POST y que haya cuerpo
        if (req.method != "POST" || req.body.empty()) {
            return R"({"error": "Invalid request"})";
        }

        // Parseamos el body de la petición como BSON
        auto body_doc = bsoncxx::from_json(req.body);
        auto doc_view = body_doc.view();

        // Validamos y extraemos los campos
        auto username_elem = doc_view["username"];
        auto email_elem = doc_view["email"];
        auto password_elem = doc_view["password"];

        if (!username_elem || !email_elem || !password_elem) {
            return R"({"error": "Missing required fields"})";
        }

        auto username = std::string(username_elem.get_string().value);
        auto email = std::string(email_elem.get_string().value);
        auto password = std::string(password_elem.get_string().value);

        // Verificamos que los campos no estén vacíos
        if (username.empty() || email.empty() || password.empty()) {
            return R"({"error": "Missing required fields"})";
        }

        // Accedemos a la colección "users"
        mongocxx::collection users = db["users"];

        // Verificamos si el usuario ya existe
        auto existing_user = users.find_one(make_document(kvp("email", email)));
        if (existing_user) {
            return R"({"error": "User already exists"})";
        }

        // Insertamos el nuevo usuario
        auto result = users.insert_one(
            make_document(
                kvp("username", username),
                kvp("email", email),
                kvp("password", password) // ¡IMPORTANTE! Hashea la contraseña antes de guardar
            )
        );

        // Verificamos si la inserción fue exitosa
        if (result && result->result().inserted_count() == 1) {
            return R"({"message": "User registered successfully"})";
        } else {
            return R"({"error": "Failed to register user"})";
        }
    } catch (const bsoncxx::exception& e) {
        // Manejo de errores de BSON
        return "{\"error\": \"Failed to parse JSON body: " + std::string(e.what()) + "\"}";
    } catch (const std::exception& e) {
        // Manejo de otros errores
        return "{\"error\": \"" + std::string(e.what()) + "\"}";
    }
}