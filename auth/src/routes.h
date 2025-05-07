#include "handlers.h"
#include "httplib.h"
#include "db.h"

void setup_routes(httplib::Server& server, mongocxx::database* db) {
    server.Post("/register", [db](const httplib::Request& req, httplib::Response& res) {
        handle_register(req, res, db);
    });
    server.Get("/userInfo", [db](const httplib::Request& req, httplib::Response& res) {
        handle_userInfo(req, res, db);
    });
    
    // server.Get("/login",handle_login);
    // server.Get("/forgot",handle_forgot);
    // server.Get("/logout",handle_logout);
    // server.Get("/refresh",handle_refresh);
      
}

