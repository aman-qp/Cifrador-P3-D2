#include "Sustitucion.h"
#include <algorithm>
#include <cctype>
#include <set>

std::string Sustitucion::encrypt(const std::string& message, const std::string& key) {
    // Validamos la clave antes de proceder
    if (!validateKey(key)) return message;
    
    // Creamos el mapa de sustitución
    auto substitutionMap = createSubstitutionMap(key);
    std::string result = message;
    
    // Aplicamos la sustitución a cada carácter
    for (char& c : result) {
        if (isalpha(c)) {
            // Preservamos mayúsculas/minúsculas
            bool isUpper = isupper(c);
            char baseChar = toupper(c);

            // Buscamos la sustitución en el mapa
            auto it = substitutionMap.find(baseChar);
            if (it != substitutionMap.end()) {
                c = isUpper ? it->second : tolower(it->second);
            }
        }
    }
    
    return result;
}

std::string Sustitucion::decrypt(const std::string& message, const std::string& key) {
    // Validamos la clave antes de proceder
    if (!validateKey(key)) return message;
    
    // Creamos el mapa de sustitución y su inverso para descifrar
    auto substitutionMap = createSubstitutionMap(key);
    auto reverseMap = createReverseMap(substitutionMap);
    std::string result = message;
    
    // Aplicamos la sustitución inversa a cada carácter
    for (char& c : result) {
        if (isalpha(c)) {
            // Preservamos mayúsculas/minúsculas
            bool isUpper = isupper(c);
            char baseChar = toupper(c);
            
            // Buscamos la sustitución inversa en el mapa
            auto it = reverseMap.find(baseChar);
            if (it != reverseMap.end()) {
                c = isUpper ? it->second : tolower(it->second);
            }
        }
    }
    
    return result;
}

bool Sustitucion::validateKey(const std::string& key) {
    return isValidSubstitutionKey(key);
}

std::map<char, char> Sustitucion::createSubstitutionMap(const std::string& key) {
    std::map<char, char> substitutionMap;
    std::string normalAlphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    
    // Creamos el mapeo entre el alfabeto normal y la clave
    for (size_t i = 0; i < normalAlphabet.length(); ++i) {
        substitutionMap[normalAlphabet[i]] = toupper(key[i]);
    }
    
    return substitutionMap;
}

std::map<char, char> Sustitucion::createReverseMap(const std::map<char, char>& substitutionMap) {
    std::map<char, char> reverseMap;
    
    // Invertimos el mapeo para el descifrado
    for (const auto& pair : substitutionMap) {
        reverseMap[pair.second] = pair.first;
    }
    
    return reverseMap;
}

bool Sustitucion::isValidSubstitutionKey(const std::string& key) {
    // La clave debe tener exactamente 26 caracteres (uno para cada letra del alfabeto)
    if (key.length() != 26) return false;
    
    std::set<char> uniqueChars;
    
    // Verificamos que cada carácter sea una letra y que no haya duplicados
    for (char c : key) {
        if (!isalpha(c)) return false;
        
        // Convertimos a mayúscula para la comparación
        char upperC = toupper(c);
        if (uniqueChars.contains(upperC)) {
            return false;  // Carácter duplicado
        }
        uniqueChars.insert(upperC);
    }
    
    return uniqueChars.size() == 26;  // Debe haber 26 caracteres únicos
}