#include "server.h"
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <fstream>

Server::Server(int port) : port(port) {
    // Crear socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        throw std::runtime_error("Error creando socket");
    }

    // Configurar dirección del servidor
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Vincular socket
    if (bind(serverSocket, reinterpret_cast<sockaddr *>(&serverAddr), sizeof(serverAddr)) < 0) {
        close(serverSocket);
        throw std::runtime_error("Error en bind");
    }
}

void Server::start() {
    // Escuchar conexiones
    if (listen(serverSocket, 3) < 0) {
        throw std::runtime_error("Error en listen");
    }

    std::cout << "Servidor escuchando en puerto " << port << std::endl;

    while (true) {
        socklen_t clientSize = sizeof(clientAddr);
        clientSocket = accept(serverSocket, reinterpret_cast<sockaddr *>(&clientAddr), &clientSize);

        if (clientSocket < 0) {
            std::cerr << "Error aceptando conexión" << std::endl;
            continue;
        }

        std::cout << "Cliente conectado" << std::endl;
        processClientMessages();
    }
}

void Server::processClientMessages() {
    while (true) {
        char buffer[1024] = {0};
        ssize_t bytesRead = recv(clientSocket, buffer, 1024, 0);

        if (bytesRead <= 0) {
            std::cout << "Cliente desconectado" << std::endl;
            displayHistory();  // Mostrar historial cuando el cliente se desconecta
            close(clientSocket);
            break;
        }

        std::string receivedData(buffer);

        if (receivedData == "EXIT:::") {
            std::cout << "Cliente solicitó desconexión" << std::endl;
            displayHistory();  // Mostrar historial cuando el cliente sale
            close(clientSocket);
            break;
        }

        size_t firstColon = receivedData.find(':');
        size_t secondColon = receivedData.find(':', firstColon + 1);

        if (firstColon != std::string::npos && secondColon != std::string::npos) {
            std::string cipherType = receivedData.substr(0, firstColon);
            std::string key = receivedData.substr(firstColon + 1, secondColon - firstColon - 1);
            std::string encryptedMessage = receivedData.substr(secondColon + 1);

            std::cout << "\nMensaje cifrado recibido: " << encryptedMessage << std::endl;
            std::cout << "Tipo de cifrado: " << cipherType << std::endl;
            std::cout << "Clave: " << key << std::endl;

            std::string decryptedMessage;
            if (cipherType == "CAESAR") {
                decryptedMessage = caesar.decrypt(encryptedMessage, key);
            } else if (cipherType == "VIGENERE") {
                decryptedMessage = vigenere.decrypt(encryptedMessage, key);
            } else if (cipherType == "SUSTITUCION") {
                decryptedMessage = sustitucion.decrypt(encryptedMessage, key);
            }

            std::cout << "Mensaje descifrado: " << decryptedMessage << std::endl;

            // Agregar al historial
            addToHistory(encryptedMessage, decryptedMessage, cipherType);
        }
    }
}

void Server::addToHistory(const std::string& encrypted, const std::string& decrypted,
                         const std::string& cipherType) const {
    HistoryRecord record{
        encrypted,
        decrypted,
        cipherType,
        getCurrentTimestamp()
    };
    saveToFile(record);
}

void Server::saveToFile(const HistoryRecord& record) const {
    std::ofstream file(historyFile, std::ios::app);  // Modo append
    if (file.is_open()) {
        file << "=== Registro ===" << std::endl;
        file << "Timestamp: " << record.timestamp << std::endl;
        file << "Tipo: " << record.cipherType << std::endl;
        file << "Cifrado: " << record.encryptedMessage << std::endl;
        file << "Descifrado: " << record.decryptedMessage << std::endl;
        file << "=================" << std::endl;
        file.close();
    } else {
        std::cerr << "Error: No se pudo abrir el archivo de historial" << std::endl;
    }
}

std::vector<HistoryRecord> Server::loadFromFile() const {
    std::vector<HistoryRecord> history;
    std::ifstream file(historyFile);
    if (file.is_open()) {
        std::string line;
        HistoryRecord currentRecord;

        while (std::getline(file, line)) {
            if (line.find("Timestamp: ") != std::string::npos) {
                currentRecord.timestamp = line.substr(11);
            } else if (line.find("Tipo: ") != std::string::npos) {
                currentRecord.cipherType = line.substr(6);
            } else if (line.find("Cifrado: ") != std::string::npos) {
                currentRecord.encryptedMessage = line.substr(9);
            } else if (line.find("Descifrado: ") != std::string::npos) {
                currentRecord.decryptedMessage = line.substr(11);
            } else if (line.find("=================") != std::string::npos) {
                history.push_back(currentRecord);
                currentRecord = HistoryRecord();
            }
        }
        file.close();
    }
    return history;
}

void Server::displayHistory() const {
    auto history = loadFromFile();

    if (history.empty()) {
        std::cout << "\nNo hay mensajes en el historial." << std::endl;
        return;
    }

    std::cout << "\n=== HISTORIAL DE MENSAJES ===" << std::endl;
    std::cout << std::setfill('=') << std::setw(80) << "=" << std::endl;
    std::cout << std::setfill(' ');

    for (size_t i = 0; i < history.size(); i++) {
        const auto& record = history[i];
        std::cout << "Registro #" << (i + 1) << std::endl;
        std::cout << "Timestamp: " << record.timestamp << std::endl;
        std::cout << "Tipo de cifrado: " << record.cipherType << std::endl;
        std::cout << "Mensaje cifrado: " << record.encryptedMessage << std::endl;
        std::cout << "Mensaje descifrado: " << record.decryptedMessage << std::endl;
        std::cout << std::setfill('-') << std::setw(80) << "-" << std::endl;
        std::cout << std::setfill(' ');
    }
}

std::string Server::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

void Server::clearHistory() const {
    std::ofstream file(historyFile, std::ios::trunc);
    file.close();
    std::cout << "Historial limpiado." << std::endl;
}