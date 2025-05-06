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
            std::string host = get_env_or_default("MONGO_HOST_" + db_name_env, "localhost");
            std::string port = get_env_or_default("MONGO_PORT_" + db_name_env, "27017");
            std::string db_name = get_env_or_default("MONGO_DATABASE_" + db_name_env, "auth");
            std::string uri = "mongodb://" + host + ":" + port;

            client_ = mongocxx::client{mongocxx::uri{uri}};
            db_ = client_[db_name];

            std::cout << "Conectado a MongoDB en: " << uri << ", base de datos: " << db_name << std::endl;
        } catch (const mongocxx::exception& e) {
            std::cerr << "Error al conectar a MongoDB: " << e.what() << std::endl;
        }
    }

    mongocxx::database& get_database() {
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
