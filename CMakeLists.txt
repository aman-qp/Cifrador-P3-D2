cmake_minimum_required(VERSION 3.28)
project(Cifrador_P3_D2)

set(CMAKE_CXX_STANDARD 20)

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
        ${ERROR_SECURITY_SOURCES}  # Añadimos los archivos de detección de errores y seguridad
)

# Ejecutable del cliente
add_executable(client
        client/client.cpp
        mains/client_main.cpp
        ${CIPHER_SOURCES}
        ${ERROR_SECURITY_SOURCES}  # Añadimos los archivos de detección de errores y seguridad
)

# Ejecutable para ver historial
add_executable(view_history
        mains/ver_historial.cpp
)

# Incluir directorios para el servidor
target_include_directories(server PRIVATE
        ${CMAKE_SOURCE_DIR}/alg_cifrado
        ${CMAKE_SOURCE_DIR}/server
        ${CMAKE_SOURCE_DIR}/detec_errores  # Añadimos el directorio de detección de errores
)

# Incluir directorios para el cliente
target_include_directories(client PRIVATE
        ${CMAKE_SOURCE_DIR}/alg_cifrado
        ${CMAKE_SOURCE_DIR}/client
        ${CMAKE_SOURCE_DIR}/detec_errores  # Añadimos el directorio de detección de errores
)
