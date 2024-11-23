#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <vector>
#include <netinet/in.h>
#include <nlohmann/json.hpp>
#include "/home/amanda/CLionProjects/Cifrador-P3-D2/alg_cifrado/Caesar.h"
#include "/home/amanda/CLionProjects/Cifrador-P3-D2/alg_cifrado/Vigenere.h"
#include "/home/amanda/CLionProjects/Cifrador-P3-D2/alg_cifrado/Sustitucion.h"

#include "/home/amanda/CLionProjects/Cifrador-P3-D2/detec_errores/ErrorDetection.h"
#include "/home/amanda/CLionProjects/Cifrador-P3-D2/detec_errores/Security.h"

using json = nlohmann::json;

struct HistoryRecord {
    std::string encryptedMessage;
    std::string decryptedMessage;
    std::string cipherType;
    std::string timestamp;

    // Convertir a JSON
    json to_json() const {
        return {
                {"timestamp", timestamp},
                {"type", cipherType},
                {"encrypted", encryptedMessage},
                {"decrypted", decryptedMessage}
        };
    }

    // Crear desde JSON
    static HistoryRecord from_json(const json& j) {
        HistoryRecord record;
        record.timestamp = j["timestamp"].get<std::string>();
        record.cipherType = j["type"].get<std::string>();
        record.encryptedMessage = j["encrypted"].get<std::string>();
        record.decryptedMessage = j["decrypted"].get<std::string>();
        return record;
    }
};

class Server {
public:
    explicit Server(int port);
    void start();

private:
    int port;
    int serverSocket, clientSocket{};
    struct sockaddr_in serverAddr{}, clientAddr{};
    Caesar caesar;
    Vigenere vigenere;
    Sustitucion sustitucion;

    const std::string historyFile = "cipher_history.json";

    void processClientMessages();
    void addToHistory(const std::string& encrypted, const std::string& decrypted,
                     const std::string& cipherType) const;
    void displayHistory() const;
    static std::string getCurrentTimestamp();
    void saveToFile(const HistoryRecord& record) const;
    void clearHistory() const;
    [[nodiscard]] std::vector<HistoryRecord> loadFromFile() const;

    [[nodiscard]] json readJsonFile() const;
    void writeJsonFile(const json& data) const;
};

#endif // SERVER_H
