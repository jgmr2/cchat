#ifndef ADD_USER_H
#define ADD_USER_H

#include "mongoConnection.h"
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/types.hpp>
#include <bcrypt/bcrypt.h>
#include <chrono>
#include <iostream>

bool add_user(MongoDBConnection& conn, const std::string& username, const std::string& password, const std::string& email) {
    try {
        if (username.empty() || password.empty() || email.empty()) {
            std::cerr << "Des données manquantes (username/email/password vacío)" << std::endl;
            return false;
        }

        auto collection = conn.get_database()["users"];

        // Verificar si ya existe el usuario por email
        bsoncxx::builder::stream::document filter_builder;
        filter_builder << "email" << email;
        auto existing = collection.find_one(filter_builder.view());

        if (existing) {
            std::cerr << "Email déjà enregistré" << std::endl;
            return false;
        }

        // Hashear la contraseña usando bcrypt
        char hashedPassword[BCRYPT_HASHSIZE];
        if (bcrypt_gensalt(12, hashedPassword) != 0) {
            std::cerr << "Error al generar la sal para el hash" << std::endl;
            return false;
        }

        char finalHash[BCRYPT_HASHSIZE];
        if (bcrypt_hashpw(password.c_str(), hashedPassword, finalHash) != 0) {
            std::cerr << "Error al hashear la contraseña" << std::endl;
            return false;
        }

        // Fecha actual
        auto now = std::chrono::system_clock::now();

        // Construir documento
        bsoncxx::builder::stream::document document{};
        document << "username" << username
                 << "email" << email
                 << "passwordHash" << finalHash
                 << "role" << "user"
                 << "createdAt" << bsoncxx::types::b_date(now);

        auto result = collection.insert_one(document.view());

        if (!result) {
            std::cerr << "Fallo al insertar el usuario" << std::endl;
            return false;
        }

        std::cout << "Utilisateur enregistré avec succès" << std::endl;
        return true;

    } catch (const std::exception& e) {
        std::cerr << "Erreur: " << e.what() << std::endl;
        return false;
    }
}

#endif // ADD_USER_H
