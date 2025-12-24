# Compilateur
CC = gcc
CFLAGS = -Wall -Wextra -I./src/gui
LDFLAGS = `sdl2-config --cflags --libs`  # <-- lien SDL2 automatique

# Dossiers
SRC_DIR = src
GUI_DIR = $(SRC_DIR)/gui
OBJ_DIR = obj

# Fichiers
SRCS = $(SRC_DIR)/main.c $(GUI_DIR)/gui.c
OBJS = $(OBJ_DIR)/main.o $(OBJ_DIR)/gui.o
TARGET = ./c_chess

# Règles par défaut
all: $(TARGET)

# Création de l'exécutable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compilation des objets
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/gui/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean
