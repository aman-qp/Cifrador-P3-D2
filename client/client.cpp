#include "client.h"
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

Client::Client(const char* serverIp, int port) : serverIp(serverIp), port(port) {
    // Crear socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        throw std::runtime_error("Error creando socket");
    }

    // Configurar dirección del servidor
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, serverIp, &serv_addr.sin_addr) <= 0) {
        throw std::runtime_error("Dirección inválida");
    }
}

void Client::connect() {
    if (::connect(sock, reinterpret_cast<sockaddr *>(&serv_addr), sizeof(serv_addr)) < 0) {
        throw std::runtime_error("Error en la conexión");
    }
    std::cout << "Conectado al servidor..." << std::endl;
}

void Client::showMenu() {
    std::cout << "\n=== MENÚ DE CIFRADO ===" << std::endl;
    std::cout << "1. César" << std::endl;
    std::cout << "2. Vigenère" << std::endl;
    std::cout << "3. Sustitución" << std::endl;
    std::cout << "4. Salir" << std::endl;
    std::cout << "Seleccione una opción: ";
}

bool Client::processOption(int choice) {
    if (choice == 4) {
        std::string exitMessage = "EXIT:::";
        send(sock, exitMessage.c_str(), exitMessage.length(), 0);
        return false;
    }

    std::string cipherInfo;

    if (choice >= 1 && choice <= 3) {
        std::string encryptedMessage;
        std::string key;
        std::string message;
        std::cout << "Ingrese el mensaje: ";
        std::getline(std::cin, message);

        std::cout << "Ingrese la clave: ";
        std::getline(std::cin, key);

        switch(choice) {
            case 1:
                if (caesar.validateKey(key)) {
                    encryptedMessage = caesar.encrypt(message, key);
                    cipherInfo = "CAESAR:" + key + ":" + encryptedMessage;
                } else {
                    std::cout << "Clave inválida para cifrado César\n";
                    return true;
                }
                break;
            case 2:
                if (vigenere.validateKey(key)) {
                    encryptedMessage = vigenere.encrypt(message, key);
                    cipherInfo = "VIGENERE:" + key + ":" + encryptedMessage;
                } else {
                    std::cout << "Clave inválida para cifrado Vigenère\n";
                    return true;
                }
                break;
            case 3:
                if (sustitucion.validateKey(key)) {
                    encryptedMessage = sustitucion.encrypt(message, key);
                    cipherInfo = "SUSTITUCION:" + key + ":" + encryptedMessage;
                } else {
                    std::cout << "Clave inválida para cifrado por Sustitución\n";
                    return true;
                }
                break;
            default: ;
        }

        send(sock, cipherInfo.c_str(), cipherInfo.length(), 0);
        std::cout << "Mensaje cifrado enviado: " << encryptedMessage << std::endl;
    } else {
        std::cout << "Opción inválida. Por favor, seleccione una opción válida." << std::endl;
    }

    return true;
}

void Client::start() {
    try {
        connect();
        bool running = true;

        while (running) {
            showMenu();
            int choice;
            std::cin >> choice;
            std::cin.ignore();

            running = processOption(choice);
        }

        close();
        std::cout << "Conexión cerrada. ¡Hasta pronto!" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void Client::close() const {
    ::close(sock);
}