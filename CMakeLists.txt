cmake_minimum_required(VERSION 3.20)

project(PT_Boids LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ==============================================================================
# 1) GoogleTest
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
# 2) SFML (Version Statique Standard)
# ==============================================================================
FetchContent_Declare(
        sfml
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.1
)

set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)

# On revient au standard : tout en statique
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(SFML_USE_STATIC_STD_LIBS ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(sfml)

# ==============================================================================
# 3) Sources
# ==============================================================================
set(BOIDS_SOURCES
        src/bd/main.cpp
        src/bd/Vec2.cpp
        src/bd/DynamicArray.cpp
        src/bd/Boids.cpp
        src/bd/Renderer.cpp
        src/bd/Flock.cpp
        src/bd/CohesionRule.cpp
        src/bd/SeparationRule.cpp
        src/bd/AlignmentRule.cpp
        src/bd/Simulation.cpp
        src/bd/InterfaceMenuSFML.cpp
)

set(BOIDS_HEADERS
        include/bd/Settings.h
        include/bd/Vec2.h
        include/bd/DynamicArray.h
        include/bd/Boids.h
        include/bd/Renderer.h
        include/bd/Rule.h
        include/bd/Flock.h
        include/bd/CohesionRule.h
        include/bd/SeparationRule.h
        include/bd/AlignmentRule.h
        include/bd/Simulation.h
        include/bd/InterfaceMenuSFML.h
        include/bd/ObstacleRule.cpp
        include/bd/ObstacleRule.h
        include/bd/ObstacleRule.h
        include/bd/PredatorRule.cpp
        include/bd/PredatorRule.h
        include/bd/PredatorRule.h
)

# ==============================================================================
# 4) Exécutable Principal
# ==============================================================================
add_executable(PT_Boids
        ${BOIDS_SOURCES}
        ${BOIDS_HEADERS}
)

target_include_directories(PT_Boids PRIVATE include)
target_link_libraries(PT_Boids PRIVATE sfml-graphics sfml-window sfml-system)

if(WIN32)
    target_link_options(PT_Boids PRIVATE -static-libgcc -static-libstdc++)
endif()

# Copie des assets
if (EXISTS "${CMAKE_SOURCE_DIR}/assets")
    add_custom_command(TARGET PT_Boids POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/assets"
            "$<TARGET_FILE_DIR:PT_Boids>/assets"
    )
endif()

# ==============================================================================
# 5) Tests
# ==============================================================================
add_executable(run_tests
        tests/test_vec2.cpp
        tests/test_dynamic_array.cpp
        tests/test_boids.cpp

        # --- AJOUT DU NOUVEAU TEST ---
        tests/test_rules.cpp

        # Sources nécessaires
        src/bd/Vec2.cpp
        src/bd/Boids.cpp
        src/bd/DynamicArray.cpp
        src/bd/Flock.cpp            # <--- Indispensable pour Flock
        src/bd/CohesionRule.cpp     # <--- Indispensable pour les règles
        src/bd/SeparationRule.cpp
        src/bd/AlignmentRule.cpp
)

target_include_directories(run_tests PRIVATE include)
target_link_libraries(run_tests PRIVATE gtest_main)

enable_testing()
include(GoogleTest)
gtest_discover_tests(run_tests)