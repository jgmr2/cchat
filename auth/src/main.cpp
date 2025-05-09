#include "crow.h"
#include <cstdlib>
#include <iostream>
#include <string>

int main()
{
    // Obtener el secreto JWT desde las variables de entorno
    const char* jwt_secret_env = std::getenv("JWT_SECRET");
    if (!jwt_secret_env) {
        std::cerr << "Error: JWT_SECRET is not set in the environment variables" << std::endl;
        return 1;
    }
    const std::string JWT_SECRET = jwt_secret_env;

    crow::SimpleApp app;

    // Ruta raíz
    CROW_ROUTE(app, "/")([](){
        return "Hello, world!";
    });

    // Ruta para verificar el secreto JWT
    CROW_ROUTE(app, "/secret")([JWT_SECRET](){
        return "JWT Secret: " + JWT_SECRET;
    });

    // Iniciar el servidor
    app.port(8080).run();
}