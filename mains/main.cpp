#include <iostream>
#include "alg_cifrado/Caesar.h"
#include "alg_cifrado/Vigenere.h"
#include "alg_cifrado/Sustitucion.h"

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

void testSustitucion() {
    Sustitucion subs;

    // La clave es un nuevo alfabeto completo (26 letras en diferente orden)
    std::string key = "QWERTYUIOPASDFGHJKLZXCVBNM";
    std::string message = "HELLO WORLD";

    if (!subs.validateKey(key)) {
        std::cout << "Clave inválida para sustitución" << std::endl;
        return;
    }

    std::string encrypted = subs.encrypt(message, key);
    std::string decrypted = subs.decrypt(encrypted, key);

    std::cout << "Prueba de Cifrado por Sustitución:" << std::endl;
    std::cout << "Mensaje original: " << message << std::endl;
    std::cout << "Clave usada: " << key << std::endl;
    std::cout << "Mensaje cifrado: " << encrypted << std::endl;
    std::cout << "Mensaje descifrado: " << decrypted << std::endl;
}

int main() {
    testCaesar();
    testVigenere();
    testSustitucion();
    return 0;
}