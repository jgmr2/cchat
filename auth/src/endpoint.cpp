#include <iostream>
#include <cstdlib>
#include <sstream>
#include <vector>

#include "httplib.h"  // https://github.com/yhirose/cpp-httplib
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <nlohmann/json.hpp>  // https://github.com/nlohmann/json

using json = nlohmann::json;

// Función que consulta la base de datos y devuelve un JSON
std::string fetch_users_as_json() {
    const char* host_env = getenv("MYSQL_HOST");
    const char* user_env = getenv("MYSQL_USER");
    const char* pass_env = getenv("MYSQL_PASSWORD");
    const char* db_env   = getenv("MYSQL_DATABASE");

    if (!host_env || !user_env || !pass_env || !db_env) {
        throw std::runtime_error("Faltan variables de entorno necesarias.");
    }

    std::string host = "tcp://" + std::string(host_env) + ":3306";

    sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
    std::unique_ptr<sql::Connection> con(driver->connect(host, user_env, pass_env));
    con->setSchema(db_env);

    std::unique_ptr<sql::Statement> stmt(con->createStatement());
    std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT * FROM users"));

    json users_json = json::array();
    while (res->next()) {
        json user = {
            {"id", res->getInt("id")},
            {"nombre", res->getString("nombre")},
            {"email", res->getString("email")},
        };
        users_json.push_back(user);
    }

    return users_json.dump(4);  // JSON bonito
}

int main() {
    httplib::Server svr;

    // Ruta /users para servir la respuesta desde MySQL
    svr.Get("/users", [](const httplib::Request& req, httplib::Response& res) {
        try {
            std::string json_str = fetch_users_as_json();
            res.set_content(json_str, "application/json");
        } catch (const std::exception& e) {
            res.status = 500;
            res.set_content(std::string("Error: ") + e.what(), "text/plain");
        }
    });

    std::cout << "Servidor corriendo en http://localhost:8080 ..." << std::endl;
    svr.listen("0.0.0.0", 8080);

    return 0;
}
