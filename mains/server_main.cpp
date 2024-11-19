#include "../server/server.h"
#include <iostream>

int main() {
    try {
        Server server(8080);
        server.start();
    } catch (const std::exception& e) {
        std::cerr << "Error del servidor: " << e.what() << std::endl;
    }
    return 0;
}