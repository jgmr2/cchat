#include "crow.h"
#include "crow/middlewares/cors.h" // Middleware CORS
#include "db.h"
#include "routes/loginUser.h"
#include "routes/registerUser.h"
#include <mongocxx/instance.hpp>
#include <cstdlib>
#include <iostream>
#include <string>

int main() {
    mongocxx::instance instance{}; // Inicializa la biblioteca MongoDB C++ driver

    // Inicializar la aplicación con el middleware CORSHandler
    crow::App<crow::CORSHandler> app;

    // Configurar las reglas de CORS globales
    auto& cors = app.get_middleware<crow::CORSHandler>();
    cors.global()
        .origin("*") // Permitir todas las solicitudes de cualquier origen
        .methods(crow::HTTPMethod::GET, crow::HTTPMethod::POST, crow::HTTPMethod::OPTIONS) // Métodos permitidos
        .headers("Content-Type", "Authorization") // Encabezados permitidos
        .max_age(3600); // Tiempo de caché para preflight

    try {
        const char* jwt_secret_env = std::getenv("JWT_SECRET");
        if (!jwt_secret_env) {
            std::cerr << "Error: JWT_SECRET is not set in the environment variables" << std::endl;
            return 1;
        }
        const std::string JWT_SECRET = jwt_secret_env;

        // Crear la conexión a MongoDB
        MongoDBConnection db_connection("AUTH");

        // Configurar las rutas
        routes::setup_register(app, db_connection, JWT_SECRET);
        routes::setup_login(app, db_connection, JWT_SECRET);

        // Iniciar el servidor
        app.port(8080).run();
    } catch (const std::exception& e) {
        std::cerr << "Error al iniciar la aplicación: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}