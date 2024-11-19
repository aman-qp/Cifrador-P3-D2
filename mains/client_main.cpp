#include "/home/amanda/CLionProjects/Cifrador-P3-D2/client/client.h"
#include <iostream>

int main() {
    try {
        Client client("127.0.0.1", 8080);
        client.start();
    } catch (const std::exception& e) {
        std::cerr << "Error del cliente: " << e.what() << std::endl;
    }
    return 0;
}