#ifndef SUSTITUCION_H
#define SUSTITUCION_H

#include "Cifrar.h"
#include <map>

class Sustitucion : public Cifrar {
public:
    // Constructor por defecto
    Sustitucion() = default;

    // Implementación de métodos de la interfaz
    std::string encrypt(const std::string& message, const std::string& key) override;
    std::string decrypt(const std::string& message, const std::string& key) override;
    bool validateKey(const std::string& key) override;

private:
    // Mapas para almacenar las sustituciones
    static std::map<char, char> createSubstitutionMap(const std::string& key);
    static std::map<char, char> createReverseMap(const std::map<char, char>& substitutionMap);

    // Metodo para verificar si una clave es válida
    static bool isValidSubstitutionKey(const std::string& key);
};

#endif