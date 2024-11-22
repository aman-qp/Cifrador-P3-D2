cmake_minimum_required(VERSION 3.28)
project(Cifrador_P3_D2)

set(CMAKE_CXX_STANDARD 20)

# Encontrar SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Archivos de los algoritmos de cifrado (compartidos)
set(CIPHER_SOURCES
        alg_cifrado/Caesar.cpp
        alg_cifrado/Vigenere.cpp
        alg_cifrado/Sustitucion.cpp
)

# Archivos de detección de errores y seguridad (compartidos)
set(ERROR_SECURITY_SOURCES
        detec_errores/ErrorDetection.cpp
        detec_errores/Security.cpp
)

# Ejecutable del servidor
add_executable(server
        server/server.cpp
        mains/server_main.cpp
        ${CIPHER_SOURCES}
        ${ERROR_SECURITY_SOURCES}
)

# Ejecutable del cliente
add_executable(client
        client/client.cpp
        mains/client_main.cpp
        ${CIPHER_SOURCES}
        ${ERROR_SECURITY_SOURCES}


)

# Ejecutable de la interfaz gráfica
add_executable(interfaz
        interfaz/ClientGUI.cpp
        interfaz/ClientGUI.h
        client/client.cpp
        client/client.h
        ${CIPHER_SOURCES}
        ${ERROR_SECURITY_SOURCES}
        mains/ClientGUImain.cpp
)

# Ejecutable para ver historial
add_executable(view_history
        mains/ver_historial.cpp
)

# Incluir directorios para el servidor
target_include_directories(server PRIVATE
        ${CMAKE_SOURCE_DIR}/alg_cifrado
        ${CMAKE_SOURCE_DIR}/server
        ${CMAKE_SOURCE_DIR}/detec_errores
)

# Incluir directorios para el cliente
target_include_directories(client PRIVATE
        ${CMAKE_SOURCE_DIR}/alg_cifrado
        ${CMAKE_SOURCE_DIR}/client
        ${CMAKE_SOURCE_DIR}/detec_errores
)

# Incluir directorios para la interfaz
target_include_directories(interfaz PRIVATE
        ${CMAKE_SOURCE_DIR}/alg_cifrado
        ${CMAKE_SOURCE_DIR}/client
        ${CMAKE_SOURCE_DIR}/detec_errores
        ${CMAKE_SOURCE_DIR}/interfaz
)

# Enlazar SFML con la interfaz
target_link_libraries(interfaz PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
)
# Add SFML links to client target
target_link_libraries(client PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
)
