#include <crow.h>
#include <mongocxx/instance.hpp>
#include "mongoConnection.h"
#include "getUsers.h"
#include "newUser.h"

int main() {
    mongocxx::instance instance{};  // ¡Debe vivir mientras dure el programa!
    crow::SimpleApp app;

    // Ruta para obtener todos los usuarios
    CROW_ROUTE(app, "/users")([]() {
        try {
            MongoDBConnection mongoConn("AUTH");
            auto usuarios = get_all_users(mongoConn);

            std::string response = "[\n";
            for (size_t i = 0; i < usuarios.size(); ++i) {
                response += usuarios[i];
                if (i < usuarios.size() - 1) response += ",\n";
            }
            response += "\n]";

            return crow::response(200, response);
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return crow::response(500, "Internal server error");
        }
    });

    // Ruta para agregar un usuario
    CROW_ROUTE(app, "/add_user").methods("POST"_method)([](const crow::request& req) {
        try {
            auto json_data = crow::json::load(req.body);
            if (!json_data || !json_data.has("username") || !json_data.has("password") || !json_data.has("email")) {
                return crow::response(400, R"({"error":"Des données manquantes"})");
            }

            std::string username = json_data["username"].s();
            std::string password = json_data["password"].s();
            std::string email = json_data["email"].s();

            MongoDBConnection mongoConn("AUTH");
            bool success = add_user(mongoConn, username, password, email);

            if (success) {
                crow::json::wvalue res;
                res["message"] = "Utilisateur enregistré avec succès";
                return crow::response(200, res);
            } else {
                return crow::response(400, R"({"error":"Email déjà enregistré ou error de inserción"})");
            }

        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return crow::response(500, R"({"error":"Internal server error"})");
        }
    });

    app.bindaddr("0.0.0.0").port(8080).multithreaded().run();
    return 0;
}
