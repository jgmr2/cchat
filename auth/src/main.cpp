#include "crow.h"

int main()
{
    crow::SimpleApp app;

    // Ruta raíz
    CROW_ROUTE(app, "/")([](){
        return "Hello, world!";
    });

    // Ruta para un saludo personalizado
    CROW_ROUTE(app, "/hello/<string>")([](const std::string& name){
        return "Hello, " + name + "!";
    });

    // Ruta para sumar dos números
    CROW_ROUTE(app, "/add/<int>/<int>")([](int a, int b){
        return crow::response(std::to_string(a + b));
    });

    // Ruta para manejar un método POST
    CROW_ROUTE(app, "/post").methods(crow::HTTPMethod::POST)([](const crow::request& req){
        return crow::response("Received POST request with body: " + req.body);
    });

    // Iniciar el servidor
    app.port(8080).run();
}