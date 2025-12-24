# Nom du binaire
BIN = c_chess

# Répertoire des sources
SRC_DIR = src

# Trouver tous les fichiers .c dans src
SRC = $(wildcard $(SRC_DIR)/*.c)

# Objets correspondants
OBJ = $(SRC:.c=.o)

# Flags SDL2
CFLAGS = $(shell sdl2-config --cflags)
LDFLAGS = $(shell sdl2-config --libs)

# Compilateur
CC = cc

# Règle par défaut
all: $(BIN)

# Lien du binaire
$(BIN): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

# Compilation des fichiers .c en .o
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

# Nettoyage
clean:
	rm -f $(OBJ) $(BIN)

distclean: clean
	rm -rf configure~ autom4te.cache aclocal.m4 Makefile.in \
	       compile install-sh missing depcomp
