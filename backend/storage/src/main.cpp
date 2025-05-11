#include "crow.h"
#include "crow/middlewares/cors.h"
#include "routes/upload.h"
#include "db.h"
#include <mongocxx/instance.hpp>
#include <cstdlib>
#include <iostream>
#include <string>

int main() {
    mongocxx::instance instance{};

    crow::App<crow::CORSHandler> app;

    auto& cors = app.get_middleware<crow::CORSHandler>();
    cors.global()
        .origin("*")
        .methods(crow::HTTPMethod::GET, crow::HTTPMethod::POST, crow::HTTPMethod::OPTIONS)
        .headers("Content-Type", "Authorization", "X-Filename")
        .max_age(3600);

    try {
        MongoDBConnection db_connection("STORAGE");
        const std::string jwt_secret = std::getenv("JWT_SECRET");
        setup_upload_routes(app, db_connection, jwt_secret);
        app.port(8080).run();
    } catch (const std::exception& e) {
        std::cerr << "Error al iniciar la aplicación: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}