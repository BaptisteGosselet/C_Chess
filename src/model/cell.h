#ifndef CELL_H
#define CELL_H

// Représente une case de l'échiquier
typedef struct {
    int color;   // 0 = blanc, 1 = noir, -1 = vide
    char piece;  // 'p','r','n','b','q','k' ou '\0' pour vide
} Cell;

#endif 
