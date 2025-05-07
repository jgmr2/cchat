// #include "httplib.h"
// #include "routes.h"
// #include <iostream>
// #include <mongocxx/instance.hpp>
// #include "db.h"

// int main() {
//     mongocxx::instance instance{};
//     MongoDBConnection db_conn("AUTH");
//     auto& db = db_conn.get_database();

//     httplib::Server server;
    
//     setup_routes(server, &db);
//     server.listen("0.0.0.0", 8080);
//     return 0;
// }
#include "httplib.h"

int main() {
    httplib::Server svr;

    svr.Get("/", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("Hello, World!", "text/plain");
    });

    svr.listen("0.0.0.0", 8080);  // Escucha en el puerto 80

    return 0;
}