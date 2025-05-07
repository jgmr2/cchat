#include <iostream>
#include <string>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/database.hpp>
#include <mongocxx/collection.hpp>
#include "../httplib.h"
#include "../json.hpp"

using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using json = nlohmann::json;

std::string getUserByEmail(const std::string& email, mongocxx::database& db) {
    try {
        if (email.empty()) {
            return R"({"error": "Missing email parameter"})";
        }

        mongocxx::collection users = db["users"];
        auto maybe_user = users.find_one(document{} << "email" << email << finalize);

        if (!maybe_user) {
            return R"({"error": "User not found"})";
        }

        json j = json::parse(bsoncxx::to_json(maybe_user->view()));
        j.erase("password");
        return j.dump(4);
    } catch (const std::exception& e) {
        return json{{"error", std::string("Exception: ") + e.what()}}.dump();
    }
}
