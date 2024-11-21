#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <vector>
#include <netinet/in.h>
#include "/home/viviana/Desktop/Cifrador-P3-D2/alg_cifrado/Caesar.h"
#include "/home/viviana/Desktop/Cifrador-P3-D2/alg_cifrado/Vigenere.h"
#include "/home/viviana/Desktop/Cifrador-P3-D2/alg_cifrado/Sustitucion.h"

#include "/home/viviana/Desktop/Cifrador-P3-D2/detec_errores/ErrorDetection.h"
#include "/home/viviana/Desktop/Cifrador-P3-D2/detec_errores/Security.h"

struct HistoryRecord {
    std::string encryptedMessage;
    std::string decryptedMessage;
    std::string cipherType;
    std::string timestamp;
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

    const std::string historyFile = "cipher_history.txt";

    void processClientMessages();
    void addToHistory(const std::string& encrypted, const std::string& decrypted,
                     const std::string& cipherType) const;
    void displayHistory() const;
    static std::string getCurrentTimestamp();
    void saveToFile(const HistoryRecord& record) const;
    void clearHistory() const;
    [[nodiscard]] std::vector<HistoryRecord> loadFromFile() const;
};

#endif // SERVER_H
