#include <iostream>
#include "/home/amanda/CLionProjects/Cifrador-P3-D2/interfaz/ClientGUI.cpp"

int main() {
    try {
        ClientGUI gui("127.0.0.1", 8080);  // Ajusta la IP y puerto según tu configuración
        gui.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}