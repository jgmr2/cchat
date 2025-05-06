#include "handlers.h"
#include "httplib.h"
#include "db.h"

void setup_routes(httplib::Server& server, mongocxx::database* db) {
    server.Post("/register", [db](const httplib::Request& req, httplib::Response& res) {
        handle_register(req, res, db);
    });
    // server.Get("/login",handle_login);
    // server.Get("/logout",handle_logout);
    // server.Get("/refresh",handle_refresh);
    // server.Get("/me",handle_me);
    // server.Get("/forgot",handle_forgot);
}

