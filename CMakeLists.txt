cmake_minimum_required(VERSION 3.29)
project(Cifrador_P3_D2)

set(CMAKE_CXX_STANDARD 20)

add_executable(Cifrador_P3_D2 main.cpp
        alg_cifrado/Caesar.cpp
        alg_cifrado/Caesar.h
        alg_cifrado/Vigenere.cpp
        alg_cifrado/Vigenere.h
        alg_cifrado/Cifrar.h
)
