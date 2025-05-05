#include <crow.h>

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([](){
        return "Hello, Crow!";
    });

    app.bindaddr("0.0.0.0").port(8080).multithreaded().run();
    return 0;
}
