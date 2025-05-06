#include <iostream>
#include <string>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/database.hpp>
#include <mongocxx/collection.hpp>
#include <mongocxx/result/insert_one.hpp>
#include "../httplib.h"
#include "../json.hpp" // Asegúrate de que la ruta a json.hpp sea correcta

using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using json = nlohmann::json; // Para facilitar el uso de la librería

std::string registerUser(const httplib::Request& req, mongocxx::database& db) {
    try {
        // Verificamos que sea un POST y que haya cuerpo
        if (req.method != "POST" || req.body.empty()) {
            return "Invalid request";
        }

        try {
            // Parseamos el body de la petición como JSON
            json body = json::parse(req.body);

            // Extraemos los valores del objeto JSON
            std::string username = body["username"].get<std::string>();
            std::string email = body["email"].get<std::string>();
            std::string password = body["password"].get<std::string>();

            // Verificamos si los campos requeridos están presentes
            if (username.empty() || email.empty() || password.empty()) {
                return "Missing required fields";
            }

            mongocxx::collection users = db["users"];

            // Verificar si el usuario ya existe
            auto existing_user = users.find_one(document{} << "email" << email << finalize);
            if (existing_user) {
                return "User already exists";
            }

            // Insertar el nuevo usuario
            auto result = users.insert_one(
                document{}
                    << "username" << username
                    << "email" << email
                    << "password" << password // ¡IMPORTANTE! Debes hashear esta contraseña antes de guardar
                    << finalize
            );

            if (result && result->result().inserted_count() == 1) {
                return "User registered successfully";
            } else {
                return "Failed to register user";
            }

        } catch (const json::parse_error& e) {
            return "Failed to parse JSON body: " + std::string(e.what());
        } catch (const json::type_error& e) {
            return "Incorrect data types in JSON body: " + std::string(e.what());
        } catch (const json::out_of_range& e) {
            return "Missing fields in JSON body";
        }

    } catch (const std::exception& e) {
        return std::string("Error: ") + e.what();
    }
}