#ifndef CLIENT_H
#define CLIENT_H

#include <netinet/in.h>
#include "/home/viviana/Desktop/Cifrador-P3-D2/alg_cifrado/Caesar.h"
#include "/home/viviana/Desktop/Cifrador-P3-D2/alg_cifrado/Vigenere.h"
#include "/home/viviana/Desktop/Cifrador-P3-D2/alg_cifrado/Sustitucion.h"

class Client {
public:
    // Constructor y metodo principal
    Client(const char* serverIp, int port);
    void start();

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

    // Métodos privados
    void connect();
    void close() const;

    static void showMenu();
    bool processOption(int choice);
};

#endif // CLIENT_H