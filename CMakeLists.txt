cmake_minimum_required(VERSION 3.20)

# Nom du projet (correspond à votre dossier racine vu dans le screenshot)
project(PT_Boids)

# Standard C++
set(CMAKE_CXX_STANDARD 17)

# ==============================================================================
# 1. TÉLÉCHARGEMENT DE GOOGLE TEST
# ==============================================================================
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE  # <--- CORRIGE LE WARNING
)

# Empêche GTest d'installer des fichiers système
set(INSTALL_GTEST OFF)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Rend GTest disponible
FetchContent_MakeAvailable(googletest)

# ==============================================================================
# 2. APPLICATION PRINCIPALE (Votre simulation Boids)
# ==============================================================================
# On liste tous les fichiers nécessaires au programme principal
add_executable(PT_Boids
        src/bd/main.cpp
        src/bd/Vec2.cpp
        src/bd/DynamicArray.cpp
        include/bd/Settings.h
        src/bd/Boids.cpp
        include/bd/Boids.h
)

# On indique où trouver les fichiers .h (dossier include)
target_include_directories(PT_Boids PRIVATE include)

# ==============================================================================
# 3. EXÉCUTABLE DE TESTS (Google Test)
# ==============================================================================
add_executable(run_tests
        tests/test_vec2.cpp
        src/bd/Vec2.cpp
        tests/test_dynamic_array.cpp
        tests/test_boids.cpp

        src/bd/Boids.cpp
)

# Les tests doivent aussi savoir où sont les .h
target_include_directories(run_tests PRIVATE include)

# On lie l'exécutable de test à la librairie Google Test
target_link_libraries(run_tests PRIVATE gtest_main)

enable_testing()
include(GoogleTest)
gtest_discover_tests(run_tests)