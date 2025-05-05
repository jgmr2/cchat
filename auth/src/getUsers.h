#ifndef GET_USER_H
#define GET_USER_H

#include "mongoConnection.h"
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <optional>
#include <vector>

std::vector<std::string> get_all_users(MongoDBConnection& conn) {
    std::vector<std::string> users_json;

    try {
        auto collection = conn.get_database()["users"];

        auto cursor = collection.find({});

        for (auto&& doc : cursor) {
            users_json.push_back(bsoncxx::to_json(doc));
        }

    } catch (const std::exception& e) {
        std::cerr << "Error al obtener usuarios: " << e.what() << std::endl;
    }

    return users_json;
}

#endif // GET_USER_H
