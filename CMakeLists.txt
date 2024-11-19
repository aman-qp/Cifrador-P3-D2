cmake_minimum_required(VERSION 3.29)
project(Cifrador_P3_D2)

set(CMAKE_CXX_STANDARD 20)

# Archivos de los algoritmos de cifrado (compartidos)
set(CIPHER_SOURCES
        alg_cifrado/Caesar.cpp
        alg_cifrado/Vigenere.cpp
        alg_cifrado/Sustitucion.cpp
)

# Ejecutable del servidor
add_executable(server
        server/server.cpp
        mains/server_main.cpp
        ${CIPHER_SOURCES}
)

# Ejecutable del cliente
add_executable(client
        client/client.cpp
        mains/client_main.cpp
        ${CIPHER_SOURCES}
)

# Ejecutable para ver historial
add_executable(view_history
        mains/ver_historial.cpp
)

# Incluir directorios para el servidor
target_include_directories(server PRIVATE
        ${CMAKE_SOURCE_DIR}/alg_cifrado
        ${CMAKE_SOURCE_DIR}/server
)

# Incluir directorios para el cliente
target_include_directories(client PRIVATE
        ${CMAKE_SOURCE_DIR}/alg_cifrado
        ${CMAKE_SOURCE_DIR}/client
)