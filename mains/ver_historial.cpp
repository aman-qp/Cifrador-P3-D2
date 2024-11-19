#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

int main() {
    const std::string historyFile = "cipher_history.txt";
    std::ifstream file(historyFile);
    
    if (!file.is_open()) {
        std::cout << "No se encontró archivo de historial." << std::endl;
        return 1;
    }

    std::string line;
    int recordCount = 0;
    
    while (std::getline(file, line)) {
        if (line.find("=== Registro ===") != std::string::npos) {
            recordCount++;
            std::cout << "\nRegistro #" << recordCount << std::endl;
        } else {
            std::cout << line << std::endl;
        }
    }

    if (recordCount == 0) {
        std::cout << "El historial está vacío." << std::endl;
    }

    file.close();
    return 0;
}