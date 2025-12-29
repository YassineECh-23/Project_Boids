cmake_minimum_required(VERSION 3.20)

# Nom du projet
project(PT_Boids)

# Standard C++ (C++17 est un bon standard moderne et compatible)
set(CMAKE_CXX_STANDARD 17)

# ==============================================================================
# 1. GOOGLE TEST (Pour tes tests unitaires)
# ==============================================================================
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(INSTALL_GTEST OFF)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# ==============================================================================
# 2. SFML (Compilation depuis les sources pour compatibilité MAXIMALE)
# ==============================================================================
FetchContent_Declare(
        sfml
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.1  # Version stable recommandée
)

# --- OPTIONS CRITIQUES POUR LA NOTE ET LA STABILITÉ ---
# On désactive ce qui ne sert pas (Audio, Réseau) pour compiler plus vite
set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)

# IMPORTANT : On compile en STATIQUE (.a / .lib) et non en DYNAMIQUE (.dll)
# Cela résout ton erreur _setjmp et rend le projet portable.
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(SFML_USE_STATIC_STD_LIBS ON CACHE BOOL "" FORCE)

# Télécharge et rend la SFML disponible
FetchContent_MakeAvailable(sfml)

# ==============================================================================
# 3. FICHIERS SOURCES DU PROJET
# ==============================================================================
# Rassemblement des sources pour éviter de les oublier
set(BOIDS_SOURCES
        src/bd/main.cpp
        src/bd/Vec2.cpp
        src/bd/DynamicArray.cpp
        src/bd/Boids.cpp
        # Ajoute tes futurs fichiers ici (Flock.cpp, Rules.cpp...)
)

set(BOIDS_HEADERS
        include/bd/Settings.h
        include/bd/Boids.h
        include/bd/DynamicArray.h
        include/bd/Vec2.h
        src/bd/Renderer.cpp
        include/bd/Renderer.h
        include/bd/Renderer.h
        include/bd/Rule.h
        src/bd/Flock.cpp
        include/bd/Flock.h
        src/bd/CohesionRule.cpp
        include/bd/CohesionRule.h
        src/bd/SeparationRule.cpp
        include/bd/SeparationRule.h
        src/bd/AlignmentRule.cpp
        include/bd/AlignmentRule.h
        # Ajoute tes futurs headers ici
)

# ==============================================================================
# 4. EXÉCUTABLE PRINCIPAL (Le Jeu)
# ==============================================================================
add_executable(PT_Boids
        ${BOIDS_SOURCES}
        ${BOIDS_HEADERS}
)

# Dossier des headers
target_include_directories(PT_Boids PRIVATE include)

# Lien avec la SFML (Statique)
# CMake gère tout seul les dépendances complexes grâce à sfml-graphics
target_link_libraries(PT_Boids PRIVATE sfml-graphics sfml-window sfml-system)

# Copie des DLLs système si nécessaire (Windows uniquement) pour éviter les surprises
if(WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
endif()

# ==============================================================================
# 5. EXÉCUTABLE DE TESTS (Google Test)
# ==============================================================================
add_executable(run_tests
        tests/test_vec2.cpp
        tests/test_dynamic_array.cpp
        tests/test_boids.cpp

        src/bd/Vec2.cpp
        src/bd/Boids.cpp
        # src/bd/DynamicArray.cpp # Décommente si besoin
)

target_include_directories(run_tests PRIVATE include)
target_link_libraries(run_tests gtest_main)

# Activation des tests dans l'IDE
enable_testing()
include(GoogleTest)
gtest_discover_tests(run_tests)