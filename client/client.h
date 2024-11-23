#ifndef CLIENT_H
#define CLIENT_H

#include <netinet/in.h>
#include "/home/amanda/CLionProjects/Cifrador-P3-D2/alg_cifrado/Caesar.h"
#include "/home/amanda/CLionProjects/Cifrador-P3-D2/alg_cifrado/Vigenere.h"
#include "/home/amanda/CLionProjects/Cifrador-P3-D2/alg_cifrado/Sustitucion.h"

class Client {
public:
    // Constructor y metodo principal
    Client(const char* serverIp, int port);
    int getSocket() const;
    void start();
    bool initializeConnection() {
        try {
            connect();
            return true;
        } catch (const std::exception& e) {
            // Handle connection error
            return false;
        }
    }

private:
    // Atributos de conexión
    const char* serverIp;
    int port;
    int sock;
    struct sockaddr_in serv_addr{};

    // Algoritmos de cifrado
    Caesar caesar;
    Vigenere vigenere;
    Sustitucion sustitucion;

    void connect();
    void close() const;

    static void showMenu();
    bool processOption(int choice);
};

#endif // CLIENT_H