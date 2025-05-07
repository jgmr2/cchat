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
    server.Get(R"(/user/(.+))", [&](const httplib::Request& req, httplib::Response& res) {
        std::string email = httplib::detail::decode_url(req.matches[1]);
        res.set_content(getUserByEmail(email, db), "application/json");
    });    
    // server.Get("/forgot",handle_forgot);
}

