#include "handlers.h"
#include "httplib.h"
#include "db.h"
#include <iostream>
#include <string>

#include "actions/registerUser.h"
#include "actions/login.h"
#include "actions/logout.h"
#include "actions/refresh.h"
#include "actions/me.h"
#include "actions/forgot.h"

void handle_register(const httplib::Request& req, httplib::Response& res, mongocxx::database* db) {
    std::string result = registerUser(req, *db);  // *db pasa la referencia
    res.set_content(result, "text/plain");
}

// void handle_login(const httplib::Request& req, httplib::Response& res, mongocxx::database* db) {
//     std::string result = login(req, *db);
//     res.set_content(result, "text/plain");
// }

// void handle_logout(const httplib::Request& req, httplib::Response& res, mongocxx::database* db) {
//     std::string result = logout(req, *db);
//     res.set_content(result, "text/plain");
// }

// void handle_refresh(const httplib::Request& req, httplib::Response& res, mongocxx::database* db) {
//     std::string result = refresh(req, *db);
//     res.set_content(result, "text/plain");
// }

// void handle_me(const httplib::Request& req, httplib::Response& res, mongocxx::database* db) {
//     std::string result = me(req, *db);
//     res.set_content(result, "text/plain");
// }

// void handle_forgot(const httplib::Request& req, httplib::Response& res, mongocxx::database* db) {
//     std::string result = forgot(req, *db);
//     res.set_content(result, "text/plain");
// }
