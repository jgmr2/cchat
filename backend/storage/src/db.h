#ifndef MONGODB_CONNECTION_H
#define MONGODB_CONNECTION_H

#include <cstdlib>
#include <iostream>
#include <string>
#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/exception/exception.hpp>

class MongoDBConnection {
public:
    MongoDBConnection(const std::string& db_name_env) {
        try {
            // Obtener la URI completa desde las variables de entorno
            std::string uri = get_env_or_default("MONGO_URI_" + db_name_env, "");
            if (uri.empty()) {
                throw std::runtime_error("MongoDB URI is not set for " + db_name_env);
            }

            std::cout << "MongoDB URI: " << uri << std::endl;

            // Crear el cliente y conectar a la base de datos
            client_ = mongocxx::client{mongocxx::uri{uri}};

            // Obtener el nombre de la base de datos desde la URI
            std::string database_name = mongocxx::uri{uri}.database();
            if (database_name.empty()) {
                throw std::runtime_error("Database name is not specified in the URI");
            }

            db_ = client_[database_name]; // Usar directamente el std::string

            if (!db_) {
                throw std::runtime_error("Failed to initialize database object");
            }

            std::cout << "Conectado a MongoDB en: " << uri << ", base de datos: " << database_name << std::endl;
        } catch (const mongocxx::exception& e) {
            std::cerr << "Error al conectar a MongoDB: " << e.what() << std::endl;
            throw;
        } catch (const std::exception& e) {
            std::cerr << "Error inesperado: " << e.what() << std::endl;
            throw;
        }
    }

    mongocxx::database& get_database() {
        if (!db_) {
            throw std::runtime_error("Database object is not initialized");
        }
        return db_;
    }

private:
    std::string get_env_or_default(const std::string& var, const std::string& def) {
        const char* val = std::getenv(var.c_str());
        return val ? std::string(val) : def;
    }

    mongocxx::client client_;
    mongocxx::database db_;
};

#endif // MONGODB_CONNECTION_H