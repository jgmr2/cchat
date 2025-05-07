#pragma once
#include "httplib.h"
#include "db.h"


void handle_register(const httplib::Request& req, httplib::Response& res, mongocxx::database* db);

// void handle_login(const httplib::Request& req, httplib::Response& res, mongocxx::database* db);
// void handle_logout(const httplib::Request& req, httplib::Response& res, mongocxx::database* db);
// void handle_refresh(const httplib::Request& req, httplib::Response& res, mongocxx::database* db);
void handle_me(const httplib::Request& req, httplib::Response& res, mongocxx::database* db);
// void handle_forgot(const httplib::Request& req, httplib::Response& res, mongocxx::database* db);

