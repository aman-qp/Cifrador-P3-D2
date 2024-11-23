#include "Sustitucion.h"
#include <algorithm>
#include <cctype>
#include <set>

std::string Sustitucion::encrypt(const std::string& message, const std::string& key) {
    if (!validateKey(key)) return message;

    auto substitutionMap = createSubstitutionMap(key);
    std::string result = message;

    for (char& c : result) {
        if (isalpha(c)) {
            bool isUpper = isupper(c);
            char baseChar = toupper(c);

            if (substitutionMap.find(baseChar) != substitutionMap.end()) {
                char substituted = substitutionMap[baseChar];
                c = isUpper ? substituted : tolower(substituted);
            }
        }
    }

    return result;
}


// El metodo decrypt necesita usar el mapa inverso correctamente
std::string Sustitucion::decrypt(const std::string& message, const std::string& key) {
    if (!validateKey(key)) return message;

    auto substitutionMap = createSubstitutionMap(key);
    auto reverseMap = createReverseMap(substitutionMap);
    std::string result = message;

    for (char& c : result) {
        if (isalpha(c)) {
            bool isUpper = isupper(c);
            char baseChar = toupper(c);

            if (reverseMap.find(baseChar) != reverseMap.end()) {
                char substituted = reverseMap[baseChar];
                c = isUpper ? substituted : tolower(substituted);
            }
        }
    }
    
    return result;
}

std::string Sustitucion::generateFullKey(const std::string& key) {
    std::string fullKey;
    std::set<char> usedChars;

    // Primero añadimos los caracteres de la clave
    for (char c : key) {
        char upperC = toupper(c);
        if (isalpha(upperC) && !usedChars.contains(upperC)) {
            fullKey += upperC;
            usedChars.insert(upperC);
        }
    }

    // Luego añadimos el resto del alfabeto que no está en la clave
    for (char c = 'A'; c <= 'Z'; c++) {
        if (!usedChars.contains(c)) {
            fullKey += c;
        }
    }

    return fullKey;
}

bool Sustitucion::validateKey(const std::string& key) {
    if (key.empty()) return false;

    // Verificar que la clave contenga al menos una letra
    bool hasLetter = false;
    for (char c : key) {
        if (isalpha(c)) {
            hasLetter = true;
            break;
        }
    }

    return hasLetter;
}


std::map<char, char> Sustitucion::createSubstitutionMap(const std::string& key) {
    std::map<char, char> substitutionMap;
    std::string fullKey = generateFullKey(key);

    // Crear el mapeo usando el alfabeto generado
    for (int i = 0; i < 26; i++) {
        char originalChar = 'A' + i;
        substitutionMap[originalChar] = fullKey[i];
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
