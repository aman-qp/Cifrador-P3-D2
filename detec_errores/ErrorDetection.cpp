#include "ErrorDetection.h"
#include <sstream>
#include <iomanip>

std::string ErrorDetection::generateChecksum(const std::string& message) {
    // Generar un checksum simple basado en la suma de los valores de los caracteres
    unsigned int sum = 0;
    for (char c : message) {
        sum += static_cast<unsigned int>(c);
    }

    // Convertir la suma en una cadena hexadecimal (opcional)
    std::stringstream ss;
    ss << std::hex << sum;
    return ss.str();  // Retorna el checksum como una cadena hexadecimal
}

bool ErrorDetection::verifyChecksum(const std::string& message, const std::string& checksum) {
    // Generar el checksum del mensaje recibido
    std::string generatedChecksum = generateChecksum(message);

    // Comparar el checksum generado con el recibido
    return generatedChecksum == checksum;
}
