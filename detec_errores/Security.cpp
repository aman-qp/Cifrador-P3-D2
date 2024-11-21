#include "Security.h"

// Función de cifrado con XOR: cada carácter del mensaje es cifrado con la clave
std::string Security::secureEncrypt(const std::string& message, const std::string& key) {
    std::string encryptedMessage = message;
    for (size_t i = 0; i < message.length(); ++i) {
        encryptedMessage[i] ^= key[i % key.length()];  // XOR entre el carácter y el valor correspondiente de la clave
    }
    return encryptedMessage;
}

// Función de descifrado con XOR: el mismo proceso para descifrar el mensaje
std::string Security::secureDecrypt(const std::string& message, const std::string& key) {
    return secureEncrypt(message, key);  // XOR con la misma clave para descifrar
}
