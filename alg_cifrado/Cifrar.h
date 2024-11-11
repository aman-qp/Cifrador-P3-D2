#ifndef CIFRAR_H
#define CIFRAR_H

#include <string>

class Cifrar {
public:
    virtual ~Cifrar() = default;
    virtual std::string encrypt(const std::string& message, const std::string& key) = 0;
    virtual std::string decrypt(const std::string& message, const std::string& key) = 0;
    virtual bool validateKey(const std::string& key) = 0;
};

#endif //CIFRAR_H