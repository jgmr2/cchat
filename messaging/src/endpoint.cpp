#include <iostream>
#include <vector>
#include <sstream>
#include "httplib.h"  // Asegúrate de tener esta biblioteca en tu proyecto

// Función para generar el array de "Hola Mundo"
std::vector<std::string> generate_hello_world(int count) {
    std::vector<std::string> result;
    for (int i = 0; i < count; ++i) {
        result.push_back("Hola Mundo desde messaging");
    }
    return result;
}

int main() {
    // Crear servidor en el puerto 8080
    httplib::Server svr;

    // Ruta para recibir el parámetro 'count' en la URL
    svr.Get("/hello", [](const httplib::Request& req, httplib::Response& res) {
        // Obtener el valor del parámetro 'count' desde la URL
        auto count_param = req.get_param_value("count");
        int count = 1; // Valor por defecto si no se pasa 'count'

        try {
            if (!count_param.empty()) {
                count = std::stoi(count_param);  // Convertir el valor de 'count' a entero
            }
        } catch (const std::exception& e) {
            res.status = 400;
            res.set_content("Invalid 'count' parameter.", "text/plain");
            return;
        }

        // Generar el array de "Hola Mundo"
        std::vector<std::string> response_array = generate_hello_world(count);

        // Crear la respuesta en formato JSON
        std::ostringstream oss;
        oss << "[";

        for (size_t i = 0; i < response_array.size(); ++i) {
            oss << "\"" << response_array[i] << "\"";
            if (i != response_array.size() - 1) {
                oss << ", ";
            }
        }
        oss << "]";

        // Configurar la respuesta como JSON
        res.set_content(oss.str(), "application/json");
    });

    // Iniciar el servidor
    std::cout << "Servidor corriendo en http://localhost:8080..." << std::endl;
    svr.listen("0.0.0.0", 8080);

    return 0;
}
