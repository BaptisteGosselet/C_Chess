# Compilateur et flags
CC = gcc
CFLAGS = -Wall -Wextra `sdl2-config --cflags`
LIBS = `sdl2-config --libs` -lSDL2_image

# Dossier de compilation des objets
OBJDIR = obj

# Trouve tous les .c dans src/ et sous-dossiers
SRCS = $(shell find src -name '*.c')

# Remplace .c par .o et ajoute le dossier obj/
OBJS = $(patsubst src/%.c, $(OBJDIR)/%.o, $(SRCS))

# Nom de l'exécutable
TARGET = c_chess

# Règle principale
all: $(TARGET)

# Linker tous les objets
$(TARGET): $(OBJS)
	@mkdir -p $(dir $(OBJS))
	$(CC) $(OBJS) -o $@ $(LIBS)

# Compilation des .c en .o
$(OBJDIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Nettoyage
clean:
	rm -rf $(OBJDIR) $(TARGET)

.PHONY: all clean
