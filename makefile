# Compilateur
CC = gcc
CFLAGS = -Wall -Wextra -I./src/gui
LDFLAGS = `sdl2-config --cflags --libs` -lSDL2_image  # <-- SDL2 + SDL2_image

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

# Compilation des objets pour src
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compilation des objets pour src/gui
$(OBJ_DIR)/%.o: $(SRC_DIR)/gui/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean
