#include "Vigenere.h"
#include <cctype>
#include <algorithm>
#include <ranges>

// Cada letra del mensaje se desplaza según la letra correspondiente de la clave
std::string Vigenere::encrypt(const std::string& message, const std::string& key) {
    if (!validateKey(key)) return message;

    // Prepara la clave para que tenga la misma longitud que el mensaje
    std::string preparedKey = prepareKey(key, message.length());
    std::string result = message;

    // Recorre cada letra del mensaje
    for (size_t i = 0; i < message.length(); ++i) {
        if (isalpha(message[i])) {  // Solo letras
            unsigned char base = isupper(message[i]) ? 'A' : 'a';

            // Calcula cuánto debe desplazar según la letra de la clave
            unsigned int shift = static_cast<unsigned char>(toupper(preparedKey[i])) - 'A';

            // Aplica el desplazamiento
            unsigned int position = static_cast<unsigned char>(message[i]) - base;
            unsigned int shifted = (position + shift) % 26;  //%26 para quedarse en el abecedario

            // Convertimos de vuelta a letra
            result[i] = static_cast<char>(base + shifted);
        }
        // Si no es letra, la dejamos igual
    }

    return result;
}

// Similar a cifrar pero resta el desplazamiento
std::string Vigenere::decrypt(const std::string& message, const std::string& key) {
    if (!validateKey(key)) return message;

    std::string preparedKey = prepareKey(key, message.length());
    std::string result = message;

    for (size_t i = 0; i < message.length(); ++i) {
        if (isalpha(message[i])) {
            unsigned char base = isupper(message[i]) ? 'A' : 'a';
            unsigned int shift = static_cast<unsigned char>(toupper(preparedKey[i])) - 'A';

            //Resta el shift en lugar de sumarlo
            unsigned int position = static_cast<unsigned char>(message[i]) - base;
            unsigned int shifted = (position + 26 - shift) % 26;

            result[i] = static_cast<char>(base + shifted);
        }
    }

    return result;
}

// Verifica que la clave sea válida (solo letras)
bool Vigenere::validateKey(const std::string& key) {
    if (key.empty()) return false;

    return std::ranges::all_of(key,
        [this](char c) { return Vigenere::isValidChar(c); });
}

// Prepara la clave para que tenga la misma longitud que el mensaje
std::string Vigenere::prepareKey(const std::string& key, size_t messageLength) {
    std::string result;
    result.reserve(messageLength);  // Reserva memoria para evitar reallocs

    // Repite la clave las veces necesarias
    for (size_t i = 0; i < messageLength; ++i) {
        // Convierte la letra a mayúscula y la agrega a la clave
        result += static_cast<char>(toupper(key[i % key.length()]));
    }

    return result;
}

// Verifica si un carácter es una letra
bool Vigenere::isValidChar(char c) {
    return isalpha(c);
}