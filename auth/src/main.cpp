#include <crow.h>
#include<mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include "message.h"

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([](){
        return message();
    });

    app.bindaddr("0.0.0.0").port(8080).multithreaded().run();
    return 0;
}
