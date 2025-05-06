#include <iostream>
#include <string>

std::string logout(int n) {
    std::string result;  // Cadena para almacenar el resultado
    for (int i = 0; i < n; i++) {
        result += "logout\n";  // Agregar "hola" a la cadena
    }
    return result;  // Devolver la cadena completa
}
