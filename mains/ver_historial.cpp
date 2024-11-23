#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main() {
    const std::string historyFile = "cipher_history.json";
    std::ifstream file(historyFile);

    if (!file.is_open()) {
        std::cout << "No se encontró archivo de historial." << std::endl;
        return 1;
    }

    try {
        json historyData = json::parse(file);

        if (historyData.empty()) {
            std::cout << "El historial está vacío." << std::endl;
            return 0;
        }

        std::cout << "\n=== HISTORIAL DE MENSAJES ===" << std::endl;
        std::cout << std::setfill('=') << std::setw(80) << "=" << std::endl;

        int recordCount = 0;
        for (const auto& record : historyData) {
            recordCount++;
            std::cout << "Registro #" << recordCount << std::endl;
            std::cout << "Timestamp: " << record["timestamp"].get<std::string>() << std::endl;
            std::cout << "Tipo de cifrado: " << record["type"].get<std::string>() << std::endl;
            std::cout << "Mensaje cifrado: " << record["encrypted"].get<std::string>() << std::endl;
            std::cout << "Mensaje descifrado: " << record["decrypted"].get<std::string>() << std::endl;
            std::cout << std::setfill('-') << std::setw(80) << "-" << std::endl;
            std::cout << std::setfill(' ');
        }

    } catch (const std::exception& e) {
        std::cout << "Error leyendo el archivo JSON: " << e.what() << std::endl;
        return 1;
    }

    file.close();
    return 0;
}