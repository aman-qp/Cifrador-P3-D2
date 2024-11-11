#include "Caesar.h"
#include <cctype>

// Cada letra se desplaza un número fijo de posiciones en el alfabeto
std::string Caesar::encrypt(const std::string& message, const std::string& key) {
    // Obtiene el número de posiciones a desplazar
    int shift = getShift(key);
    std::string result = message;

    // Recorre cada letra
    for (char& c : result) {
        if (isalpha(c)) {  // Solo cifra letras
            char base = isupper(c) ? 'A' : 'a';

            // Realiza el desplazamiento, %26 para mantenerse en el abecedario
            int shifted = ((c - base + shift) % 26 + 26) % 26;
            c = static_cast<char>(base + shifted);
        }
        // Si no es letra queda igual
    }
    return result;
}

// Similar a encrypt, pero desplaza en dirección contraria
std::string Caesar::decrypt(const std::string& message, const std::string& key) {
    int shift = getShift(key);
    std::string result = message;

    for (char& c : result) {
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            // La diferencia con encrypt es que resta el shift
            int shifted = ((c - base - shift) % 26 + 26) % 26;
            c = static_cast<char>(base + shifted);
        }
    }
    return result;
}

// Verifica que la clave sea un número válido (0-25)
bool Caesar::validateKey(const std::string& key) {
    try {
        int shift = std::stoi(key);
        return shift >= 0 && shift < 26;
    } catch (...) {
        return false;
    }
}

// Convierte la clave string a un número entre 0 y 25
int Caesar::getShift(const std::string& key) {
    try {
        return std::stoi(key) % 26;
    } catch (...) {
        return 0;
    }
}
