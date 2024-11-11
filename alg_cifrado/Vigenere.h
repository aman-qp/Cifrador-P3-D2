#ifndef VIGENERE_H
#define VIGENERE_H

#include "Cifrar.h"

class Vigenere : public Cifrar {
public:
    Vigenere() = default;
    std::string encrypt(const std::string& message, const std::string& key) override;
    std::string decrypt(const std::string& message, const std::string& key) override;
    bool validateKey(const std::string& key) override;

private:
    static std::string prepareKey(const std::string& key, size_t messageLength);
    static bool isValidChar(char c);
};

#endif //VIGENERE_H
