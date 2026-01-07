# ==========================================
# 1. CONFIGURATION GÉNÉRALE
# ==========================================
EXEC = boids
TEST_EXEC = run_tests

CXX = g++
# Options standards (C++17, Warnings, Optimisation)
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude -MMD -MP

# Dossiers du projet
SRC_DIR = src/bd
INC_DIR = include/bd
TEST_DIR = tests
OBJ_DIR = build
BIN_DIR = bin

# ==========================================
# 2. CONFIGURATION GOOGLETEST (LOCAL)
# ==========================================
GTEST_DIR = lib/googletest
GTEST_SRC = $(GTEST_DIR)/googletest
GMOCK_SRC = $(GTEST_DIR)/googlemock

# Où on va compiler les objets de gtest
GTEST_OBJ_DIR = $(OBJ_DIR)/gtest

# Flags spécifiques pour inclure gtest localement
TEST_CXXFLAGS = $(CXXFLAGS) -I$(GTEST_SRC)/include -I$(GMOCK_SRC)/include -pthread

# Les fichiers .a qu'on va générer nous-mêmes
LIB_GTEST = $(BIN_DIR)/libgtest.a
LIB_GTEST_MAIN = $(BIN_DIR)/libgtest_main.a

# ==========================================
# 3. FICHIERS SOURCES DU PROJET
# ==========================================

# JEU : Tous les .cpp dans src/bd
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# TESTS : Tous les .cpp dans tests/
TEST_SRCS = $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJS = $(patsubst $(TEST_DIR)/%.cpp, $(OBJ_DIR)/tests/%.o, $(TEST_SRCS))

# CORE : Tout le jeu sauf le main.cpp (pour éviter le conflit avec le main des tests)
MAIN_OBJ = $(OBJ_DIR)/main.o
CORE_OBJS = $(filter-out $(MAIN_OBJ), $(OBJS))

# Dépendances auto
DEPS = $(OBJS:.o=.d) $(TEST_OBJS:.o=.d)

# Bibliothèques SFML
LDFLAGS_GAME = -lsfml-graphics -lsfml-window -lsfml-system

# ==========================================
# 4. RÈGLES PRINCIPALES
# ==========================================

all: directories game

directories:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/tests
	@mkdir -p $(GTEST_OBJ_DIR)
	@mkdir -p $(BIN_DIR)

# --- JEU ---
game: $(BIN_DIR)/$(EXEC)

$(BIN_DIR)/$(EXEC): $(OBJS)
	@echo ">> Linkage du JEU..."
	$(CXX) $(OBJS) -o $@ $(LDFLAGS_GAME)
	@echo ">> Jeu prêt : $(BIN_DIR)/$(EXEC)"

# --- TESTS ---
# Les tests dépendent des objets de tests, du coeur du jeu, ET de la librairie gtest locale
tests: directories $(LIB_GTEST_MAIN) $(BIN_DIR)/$(TEST_EXEC)

$(BIN_DIR)/$(TEST_EXEC): $(CORE_OBJS) $(TEST_OBJS) $(LIB_GTEST_MAIN) $(LIB_GTEST)
	@echo ">> Linkage des TESTS..."
	$(CXX) $(CORE_OBJS) $(TEST_OBJS) $(LIB_GTEST_MAIN) $(LIB_GTEST) -o $@ $(LDFLAGS_GAME) -pthread
	@echo ">> Tests prêts : $(BIN_DIR)/$(TEST_EXEC)"

# ==========================================
# 5. COMPILATION DE GOOGLETEST (LOCALE)
# ==========================================

# Compile gtest-all.cc -> gtest-all.o
$(GTEST_OBJ_DIR)/gtest-all.o:
	@echo ">> Compilation de GoogleTest (Local)..."
	$(CXX) $(TEST_CXXFLAGS) -I$(GTEST_SRC) -I$(GMOCK_SRC) -c $(GTEST_SRC)/src/gtest-all.cc -o $@

# Compile gtest_main.cc -> gtest_main.o
$(GTEST_OBJ_DIR)/gtest_main.o:
	@echo ">> Compilation de GoogleTest Main..."
	$(CXX) $(TEST_CXXFLAGS) -I$(GTEST_SRC) -I$(GMOCK_SRC) -c $(GTEST_SRC)/src/gtest_main.cc -o $@

# Archive en libgtest.a
$(LIB_GTEST): $(GTEST_OBJ_DIR)/gtest-all.o
	@ar -rv $@ $^ > /dev/null 2>&1

# Archive en libgtest_main.a
$(LIB_GTEST_MAIN): $(GTEST_OBJ_DIR)/gtest-all.o $(GTEST_OBJ_DIR)/gtest_main.o
	@ar -rv $@ $^ > /dev/null 2>&1

# ==========================================
# 6. COMPILATION DES SOURCES
# ==========================================

# Fichiers .cpp du jeu
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compil jeu: $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Fichiers .cpp des tests (doivent inclure les headers gtest)
$(OBJ_DIR)/tests/%.o: $(TEST_DIR)/%.cpp
	@echo "Compil test: $<"
	$(CXX) $(TEST_CXXFLAGS) -c $< -o $@

-include $(DEPS)

# ==========================================
# 7. COMMANDES
# ==========================================

run: game
	@echo ">> Lancement..."
	./$(BIN_DIR)/$(EXEC)

run_tests: tests
	@echo ">> Exécution des tests..."
	./$(BIN_DIR)/$(TEST_EXEC)

clean:
	@echo ">> Nettoyage..."
	rm -rf $(OBJ_DIR) $(BIN_DIR)

cleanid:
	@find . -type f -name "*:Zone.Identifier" -exec rm -f {} +

rebuild: clean all

.PHONY: all directories game tests run run_tests clean cleanid rebuild