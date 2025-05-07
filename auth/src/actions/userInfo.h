#pragma once
#include "httplib.h"
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/exception/query_exception.hpp>
#include <string>

std::string userInfo(const httplib::Request& req, mongocxx::database& db) {
    try {
        // Verificar si el parámetro "user_id" está presente en la solicitud
        if (!req.has_param("user_id")) {
            return R"({"error": "Missing user_id parameter"})";
        }

        // Obtener el parámetro "user_id" de la solicitud
        std::string user_id = req.get_param_value("user_id");

        // Crear un filtro para buscar al usuario en la base de datos
        bsoncxx::builder::stream::document filter_builder;
        filter_builder << "_id" << bsoncxx::oid(user_id);

        // Buscar el usuario en la colección "users"
        auto collection = db["users"];
        auto user_doc = collection.find_one(filter_builder.view());

        if (user_doc) {
            // Convertir el documento BSON a JSON y devolverlo
            return bsoncxx::to_json(user_doc->view());
        } else {
            // Usuario no encontrado
            return R"({"error": "User not found"})";
        }
    } catch (const mongocxx::query_exception& e) {
        // Manejar errores de consulta
        return R"({"error": "Database query error"})";
    } catch (const std::exception& e) {
        // Manejar otros errores
        return R"({"error": "An unexpected error occurred"})";
    }
}