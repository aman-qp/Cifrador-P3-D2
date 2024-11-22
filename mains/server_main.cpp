#include "/home/mau/Desktop/Cifrador-P3-D2/server/server.h"
#include <iostream>

int main() {
    try {
        Server server(8080); 
        std::cout << "Iniciando servidor en el puerto 8080..." << std::endl;
        server.start();
    } catch (const std::exception& e) {
        std::cerr << "Error del servidor: " << e.what() << std::endl;
    }
    return 0;
}
