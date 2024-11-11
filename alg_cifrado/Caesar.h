#ifndef CAESAR_H
#define CAESAR_H

#include "Cifrar.h"

class Caesar : public Cifrar {
public:
    Caesar() = default;
    std::string encrypt(const std::string& message, const std::string& key) override;
    std::string decrypt(const std::string& message, const std::string& key) override;
    bool validateKey(const std::string& key) override;

private:
    static int getShift(const std::string& key);
};

#endif //CAESAR_H
