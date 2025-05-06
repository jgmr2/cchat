#include "httplib.h"
#include <mongocxx/instance.hpp>
#include "mongoConnection.h"
#include "getUsers.h"
#include "newUser.h"
#include <iostream>

int main() {
    mongocxx::instance instance{};  // Debe vivir mientras dure el programa
    httplib::Server server;

    // Ruta para obtener todos los usuarios
    server.Get("/users", [](const httplib::Request&, httplib::Response& res) {
        try {
            MongoDBConnection mongoConn("AUTH");
            auto usuarios = get_all_users(mongoConn);

            std::string response = "[\n";
            for (size_t i = 0; i < usuarios.size(); ++i) {
                response += usuarios[i];
                if (i < usuarios.size() - 1) response += ",\n";
            }
            response += "\n]";

            res.set_content(response, "application/json");
            res.status = 200;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            res.status = 500;
            res.set_content("Internal server error", "text/plain");
        }
    });

    // Ruta para agregar un usuario
    server.Post("/add_user", [](const httplib::Request& req, httplib::Response& res) {
        try {
            std::string body = req.body;

            // Extracción manual de campos (sin JSON parsing)
            auto get_value = [&](const std::string& key) -> std::string {
                auto pos = body.find("\"" + key + "\"");
                if (pos == std::string::npos) return "";
                auto start = body.find(":", pos);
                auto quote1 = body.find("\"", start);
                auto quote2 = body.find("\"", quote1 + 1);
                if (quote1 == std::string::npos || quote2 == std::string::npos) return "";
                return body.substr(quote1 + 1, quote2 - quote1 - 1);
            };

            std::string username = get_value("username");
            std::string password = get_value("password");
            std::string email = get_value("email");

            if (username.empty() || password.empty() || email.empty()) {
                res.status = 400;
                res.set_content(R"({"error":"Des données manquantes"})", "application/json");
                return;
            }

            MongoDBConnection mongoConn("AUTH");
            bool success = add_user(mongoConn, username, password, email);

            if (success) {
                res.status = 200;
                res.set_content(R"({"message":"Utilisateur enregistré avec succès"})", "application/json");
            } else {
                res.status = 400;
                res.set_content(R"({"error":"Email déjà enregistré ou error de inserción"})", "application/json");
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            res.status = 500;
            res.set_content(R"({"error":"Internal server error"})", "application/json");
        }
    });

    std::cout << "Servidor escuchando en http://0.0.0.0:8080" << std::endl;
    server.listen("0.0.0.0", 8080);
    return 0;
}
