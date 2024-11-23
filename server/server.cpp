#include "server.h"
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <fstream>

#include "/home/amanda/CLionProjects/Cifrador-P3-D2/detec_errores/ErrorDetection.h"
#include "/home/amanda/CLionProjects/Cifrador-P3-D2/detec_errores/Security.h"

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
            displayHistory();
            close(clientSocket);
            break;
        }

        std::string receivedData(buffer);

        // Verificar si el mensaje es el comando EXIT
        if (receivedData == "EXIT:::") {
            std::cout << "Cliente solicitó desconexión" << std::endl;
            displayHistory();
            close(clientSocket);
            break;
        }

        std::cout << "Datos recibidos del cliente: " << receivedData << std::endl;  // Mensaje de depuración

        // Dividir el mensaje en partes: tipo de cifrado, clave, mensaje cifrado y checksum
        size_t firstColon = receivedData.find(':');
        size_t secondColon = receivedData.find(':', firstColon + 1);
        size_t thirdColon = receivedData.find(':', secondColon + 1);
        size_t lastColon = receivedData.find(':', thirdColon + 1);

        if (firstColon != std::string::npos && secondColon != std::string::npos &&
            thirdColon != std::string::npos && lastColon != std::string::npos) {

            std::string cipherType = receivedData.substr(0, firstColon);  // Tipo de cifrado
            std::string key = receivedData.substr(firstColon + 1, secondColon - firstColon - 1);  // Clave
            std::string encryptedMessage = receivedData.substr(secondColon + 1, thirdColon - secondColon - 1);  // Mensaje cifrado
            std::string checksum = receivedData.substr(thirdColon + 1, lastColon - thirdColon - 1);  // Checksum

            std::cout << "\nMensaje cifrado recibido: " << encryptedMessage << std::endl;
            std::cout << "Tipo de cifrado: " << cipherType << std::endl;
            std::cout << "Clave: " << key << std::endl;
            std::cout << "Checksum recibido: " << checksum << std::endl;

            // Verificación de errores (Checksum)
            if (!ErrorDetection::verifyChecksum(encryptedMessage, checksum)) {
                std::cerr << "Error: El checksum no coincide, el mensaje podría estar corrupto." << std::endl;
                std::string errorMessage = "ERROR: El checksum no coincide.";
                send(clientSocket, errorMessage.c_str(), errorMessage.length(), 0);
                continue; // Ignora el mensaje corrupto
            }

            // Verificación de la clave
            bool isKeyValid = false;
            std::cout << "Verificando clave" << std::endl;  // Mensaje de depuración
            if (cipherType == "CAESAR") {
                isKeyValid = caesar.validateKey(key);
            } else if (cipherType == "VIGENERE") {
                isKeyValid = vigenere.validateKey(key);
            } else if (cipherType == "SUSTITUCION") {
                isKeyValid = sustitucion.validateKey(key);
            } else if (cipherType == "XOR") {
                isKeyValid = true;  // XOR no necesita validación adicional de clave
            }

            // Enviar respuesta al cliente si la clave es incorrecta
            if (!isKeyValid) {
                std::cout << "ERROR: Clave incorrecta recibida del cliente." << std::endl;  // Mensaje de error en la terminal del servidor
                std::string errorMessage = "ERROR: Clave incorrecta.";
                send(clientSocket, errorMessage.c_str(), errorMessage.length(), 0);
                continue; // Continuar esperando otro mensaje
            }

            // Si la clave es correcta, proceder con el descifrado
            std::string decryptedMessage;
            if (cipherType == "CAESAR") {
                decryptedMessage = caesar.decrypt(encryptedMessage, key);
            } else if (cipherType == "VIGENERE") {
                decryptedMessage = vigenere.decrypt(encryptedMessage, key);
            } else if (cipherType == "SUSTITUCION") {
                decryptedMessage = sustitucion.decrypt(encryptedMessage, key);
            } else if (cipherType == "XOR") {
                // Descifrado con XOR utilizando la clase Security
                Security security;
                decryptedMessage = security.secureDecrypt(encryptedMessage, key);
            }

            // Mostrar mensaje descifrado
            std::cout << "Mensaje descifrado: " << decryptedMessage << std::endl;

            addToHistory(encryptedMessage, decryptedMessage, cipherType);

            // Enviar el mensaje descifrado de vuelta al cliente
            std::string response = "Mensaje descifrado: " + decryptedMessage;
            send(clientSocket, response.c_str(), response.length(), 0);
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
    json historyArray = readJsonFile();
    historyArray.push_back(record.to_json());
    writeJsonFile(historyArray);
}

std::string Server::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
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

json Server::readJsonFile() const {
    std::ifstream file(historyFile);
    if (!file.is_open()) {
        return json::array();
    }
    try {
        json data = json::parse(file);
        return data;
    } catch (...) {
        return json::array();
    }
}

void Server::writeJsonFile(const json& data) const {
    std::ofstream file(historyFile);
    if (file.is_open()) {
        file << std::setw(4) << data << std::endl;
    }
}

void Server::clearHistory() const {
    writeJsonFile(json::array());
}

std::vector<HistoryRecord> Server::loadFromFile() const {
    std::vector<HistoryRecord> records;
    json historyArray = readJsonFile();

    for (const auto& recordJson : historyArray) {
        records.push_back(HistoryRecord::from_json(recordJson));
    }

    return records;
}