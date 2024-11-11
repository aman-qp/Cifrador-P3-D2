#include <iostream>
#include "alg_cifrado/Caesar.h"
#include "alg_cifrado/Vigenere.h"

void testCaesar() {
    Caesar caesar;
    std::string message = "hola me llamo amanda";
    std::string key = "5";

    std::cout << "\nPrueba Cifrado César:" << std::endl;
    std::cout << "Mensaje original: " << message << std::endl;

    std::string encrypted = caesar.encrypt(message, key);
    std::cout << "Mensaje cifrado: " << encrypted << std::endl;

    std::string decrypted = caesar.decrypt(encrypted, key);
    std::cout << "Mensaje descifrado: " << decrypted << std::endl;
}

void testVigenere() {
    Vigenere vigenere;
    std::string message = "HELLO WORLD";
    std::string key = "hola";

    std::cout << "\nPrueba Cifrado Vigenère:" << std::endl;
    std::cout << "Mensaje original: " << message << std::endl;

    std::string encrypted = vigenere.encrypt(message, key);
    std::cout << "Mensaje cifrado: " << encrypted << std::endl;

    std::string decrypted = vigenere.decrypt(encrypted, key);
    std::cout << "Mensaje descifrado: " << decrypted << std::endl;
}

int main() {
    testCaesar();
    testVigenere();
    return 0;
}