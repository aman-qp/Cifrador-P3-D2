#ifndef ERRORDETECTION_H
#define ERRORDETECTION_H

#include <string>

class ErrorDetection {
public:
    // Método para generar el checksum del mensaje
    static std::string generateChecksum(const std::string& message);

    // Método para verificar si el checksum es correcto
    static bool verifyChecksum(const std::string& message, const std::string& checksum);
};

#endif // ERRORDETECTION_H
