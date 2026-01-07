# ==========================================
# MAKEFILE POUR LE BENCHMARK (Dossier Bench)
# ==========================================

EXEC = run_bench
CXX = g++

# OPTIONS DE COMPILATION
# -Iinclude    : Pour trouver "bd/Flock.h"
# -Iinclude/bd : Pour trouver "Flock.h" directement (si les fichiers s'incluent entre eux)
CXXFLAGS = -std=c++17 -Wall -Wextra -O3 -Iinclude -Iinclude/bd

# LIBS (Benchmark + SFML si besoin pour éviter les erreurs de compilation)
LDFLAGS = -lbenchmark -lpthread -lsfml-graphics -lsfml-window -lsfml-system

# DOSSIERS (Relatifs à l'endroit où est ce Makefile)
SRC_DIR = src/bd
OBJ_DIR = build
BIN_DIR = bin

# ==========================================
# LISTE DES FICHIERS
# ==========================================

# 1. Le fichier principal du benchmark
MAIN_SRC = main_bench.cpp
MAIN_OBJ = $(OBJ_DIR)/main_bench.o

# 2. Les fichiers du moteur (dans Bench/src/bd)

CORE_SRCS = $(SRC_DIR)/Flock.cpp \
            $(SRC_DIR)/Boids.cpp \
            $(SRC_DIR)/CohesionRule.cpp \
            $(SRC_DIR)/SeparationRule.cpp \
            $(SRC_DIR)/AlignmentRule.cpp \
            $(SRC_DIR)/ObstacleRule.cpp \
            $(SRC_DIR)/PredatorRule.cpp \
            $(SRC_DIR)/DynamicArray.cpp
          

# Génération des noms .o
CORE_OBJS = $(CORE_SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# ==========================================
# RÈGLES DE COMPILATION
# ==========================================

all: $(BIN_DIR)/$(EXEC)

# Création de l'exécutable
$(BIN_DIR)/$(EXEC): $(MAIN_OBJ) $(CORE_OBJS)
	@echo ">> Linkage de l'executable..."
	@mkdir -p $(BIN_DIR)
	$(CXX) $^ -o $@ $(LDFLAGS)

# Compilation du main_bench.cpp
$(OBJ_DIR)/main_bench.o: $(MAIN_SRC)
	@echo ">> Compilation du Main..."
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compilation des sources du moteur (src/bd/*.cpp)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo ">> Compilation de $<..."
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Commandes
run: all
	@echo ">> Lancement du Benchmark..."
	./$(BIN_DIR)/$(EXEC)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all run clean