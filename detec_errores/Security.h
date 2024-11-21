#ifndef SECURITY_H
#define SECURITY_H

#include <string>

class Security {
public:
    // Cifra un mensaje usando XOR con una clave secreta
    static std::string secureEncrypt(const std::string& message, const std::string& key);

    // Descifra un mensaje usando XOR con una clave secreta
    static std::string secureDecrypt(const std::string& message, const std::string& key);
};

#endif // SECURITY_H
