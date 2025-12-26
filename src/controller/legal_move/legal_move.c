#include "../../model/move.h"
#include "../../model/pieces_type.h"
#include "../../model/cell.h"
#include "../../model/color.h"
#include "../../model/position.h"
#include <stdbool.h>
#include <stdio.h>

/**
 * Retourne combien de fois la case (x, y) est attaquée par la couleur `byColor`
 */
int countAttacksOnSquare(const Position *pos, int x, int y, Color byColor) {
    int count = 0;

    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            Cell cell = pos->board_model[i][j];
            if(cell.color != byColor) continue;

            switch(cell.piece){
                case PIECE_PAWN: {
                    int dir = (byColor == COLOR_WHITE) ? -1 : 1;
                    for(int dj=-1; dj<=1; dj+=2){
                        int nx = i + dir;
                        int ny = j + dj;
                        if(nx==x && ny==y) count++;
                    }
                    break;
                }
                case PIECE_KNIGHT: {
                    int offsets[8][2] = {{-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1}};
                    for(int k=0;k<8;k++){
                        int nx=i+offsets[k][0], ny=j+offsets[k][1];
                        if(nx==x && ny==y) count++;
                    }
                    break;
                }
                case PIECE_BISHOP:
                case PIECE_ROOK:
                case PIECE_QUEEN: {
                    int dirs[8][2] = {
                        {-1,-1},{-1,1},{1,-1},{1,1},
                        {-1,0},{1,0},{0,-1},{0,1}
                    };
                    int maxDir = (cell.piece == PIECE_BISHOP) ? 4 :
                                 (cell.piece == PIECE_ROOK) ? 4 : 8;
                    for(int d=0; d<maxDir; d++){
                        int dx = dirs[d][0], dy = dirs[d][1];
                        int nx = i+dx, ny=j+dy;
                        while(nx>=0 && nx<8 && ny>=0 && ny<8){
                            Cell c = pos->board_model[nx][ny];
                            if(nx==x && ny==y){
                                count++;
                                break;
                            }
                            if(c.piece != PIECE_NONE) break;
                            nx+=dx; ny+=dy;
                        }
                    }
                    break;
                }
                case PIECE_KING: {
                    for(int dx=-1; dx<=1; dx++){
                        for(int dy=-1; dy<=1; dy++){
                            if(dx==0 && dy==0) continue;
                            if(i+dx==x && j+dy==y) count++;
                        }
                    }
                    break;
                }
                default: break;
            }
        }
    }

    return count;
}

/* --- Fonctions pour gérer les coups --- */

bool movesEqual(Move a, Move b) {
    return a.fromX == b.fromX
        && a.fromY == b.fromY
        && a.toX   == b.toX
        && a.toY   == b.toY
        && a.promotion == b.promotion;
}

void addMoveToList(int fromX, int fromY, int toX, int toY, int promotion, MoveList *movesList) {
    if (movesList->count >= MAX_MOVES) return;
    movesList->moves[movesList->count++] = (Move){
        .fromX = fromX,
        .fromY = fromY,
        .toX = toX,
        .toY = toY,
        .promotion = promotion
    };
}

void resetMovesList(MoveList *movesList) {
    movesList->count = 0;
}

/* --- Simulation de coups pour filtrer l'échec --- */
void makeMove(Position *pos, Move move, Cell *captured) {
    *captured = pos->board_model[move.toX][move.toY];
    pos->board_model[move.toX][move.toY] = pos->board_model[move.fromX][move.fromY];
    pos->board_model[move.fromX][move.fromY] = EMPTY_CELL;
}

void undoMove(Position *pos, Move move, Cell captured) {
    pos->board_model[move.fromX][move.fromY] = pos->board_model[move.toX][move.toY];
    pos->board_model[move.toX][move.toY] = captured;
}

bool isKingInCheck(const Position *pos, Color color) {
    int kingX = -1, kingY = -1;
    for (int i=0;i<8;i++){
        for (int j=0;j<8;j++){
            Cell cell = pos->board_model[i][j];
            if(cell.piece==PIECE_KING && cell.color==color){
                kingX=i; kingY=j; break;
            }
        }
    }
    if(kingX==-1) return true; // roi absent
    Color enemy = (color==COLOR_WHITE)?COLOR_BLACK:COLOR_WHITE;
    return countAttacksOnSquare(pos, kingX, kingY, enemy) > 0;
}

bool getKingPosition(const Position *pos, Color color, int *kingX, int *kingY) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Cell cell = pos->board_model[i][j];
            if (cell.piece == PIECE_KING && cell.color == color) {
                *kingX = i;
                *kingY = j;
                return true;
            }
        }
    }
    return false;
}

void listSlidingMoves(Position *pos, int oI, int oJ, int di, int dj, Color color, MoveList *movesList) {
    int i = oI + di;
    int j = oJ + dj;

    while (i>=0 && i<=7 && j>=0 && j<=7) {
        Cell cell = pos->board_model[i][j];
        if (cell.piece == PIECE_NONE) addMoveToList(oI,oJ,i,j,0,movesList);
        else {
            if (cell.color != color) addMoveToList(oI,oJ,i,j,0,movesList);
            break;
        }
        i += di; j += dj;
    }
}

void listPawnMoves(Position *pos, int oI, int oJ, Color color, MoveList *movesList) {
    int dir = (color == COLOR_WHITE)?-1:1;
    int startRow = (color == COLOR_WHITE)?6:1;
    int nextRow = oI + dir;
    if(nextRow<0||nextRow>7) return;

    if(pos->board_model[nextRow][oJ].piece==PIECE_NONE) addMoveToList(oI,oJ,nextRow,oJ,0,movesList);
    int doubleRow = oI + 2*dir;
    if(oI==startRow && pos->board_model[doubleRow][oJ].piece==PIECE_NONE)
        addMoveToList(oI,oJ,doubleRow,oJ,0,movesList);

    for(int dj=-1;dj<=1;dj+=2){
        int j=oJ+dj;
        if(j>=0 && j<=7){
            Cell target = pos->board_model[nextRow][j];
            if(target.piece!=PIECE_NONE && target.color!=color)
                addMoveToList(oI,oJ,nextRow,j,0,movesList);
        }
    }
}

void listRookMoves(Position *pos,int oI,int oJ,Color color,MoveList *movesList){
    listSlidingMoves(pos,oI,oJ,-1,0,color,movesList);
    listSlidingMoves(pos,oI,oJ,1,0,color,movesList);
    listSlidingMoves(pos,oI,oJ,0,-1,color,movesList);
    listSlidingMoves(pos,oI,oJ,0,1,color,movesList);
}

void listBishopMoves(Position *pos,int oI,int oJ,Color color,MoveList *movesList){
    listSlidingMoves(pos,oI,oJ,-1,-1,color,movesList);
    listSlidingMoves(pos,oI,oJ,-1,1,color,movesList);
    listSlidingMoves(pos,oI,oJ,1,-1,color,movesList);
    listSlidingMoves(pos,oI,oJ,1,1,color,movesList);
}

void listQueenMoves(Position *pos,int oI,int oJ,Color color,MoveList *movesList){
    listRookMoves(pos,oI,oJ,color,movesList);
    listBishopMoves(pos,oI,oJ,color,movesList);
}

void listKnightMoves(Position *pos,int oI,int oJ,Color color,MoveList *movesList){
    int offsets[8][2]={{-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1}};
    for(int k=0;k<8;k++){
        int i=oI+offsets[k][0];
        int j=oJ+offsets[k][1];
        if(i>=0&&i<=7 && j>=0&&j<=7){
            Cell cell = pos->board_model[i][j];
            if(cell.piece==PIECE_NONE||cell.color!=color) addMoveToList(oI,oJ,i,j,0,movesList);
        }
    }
}

void listKingMoves(Position *pos,int oI,int oJ,Color color,MoveList *movesList){
    for(int i=oI-1;i<=oI+1;i++){
        for(int j=oJ-1;j<=oJ+1;j++){
            if(i>=0 && i<=7 && j>=0 && j<=7 && !(i==oI && j==oJ)){
                Cell cell = pos->board_model[i][j];
                if(cell.piece==PIECE_NONE||cell.color!=color) addMoveToList(oI,oJ,i,j,0,movesList);
            }
        }
    }
    // Roque à implémenter si besoin
}

/* --- Génération puis filtrage des coups légaux --- */
void updateAllLegalMoves(Position *pos, Color color, MoveList *movesList) {
    resetMovesList(movesList);
    MoveList pseudo;
    resetMovesList(&pseudo);

    // Générer tous les coups pseudo-légaux
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            Cell cell = pos->board_model[i][j];
            if(cell.color==color){
                switch(cell.piece){
                    case PIECE_PAWN: listPawnMoves(pos,i,j,color,&pseudo); break;
                    case PIECE_ROOK: listRookMoves(pos,i,j,color,&pseudo); break;
                    case PIECE_BISHOP:listBishopMoves(pos,i,j,color,&pseudo); break;
                    case PIECE_KNIGHT:listKnightMoves(pos,i,j,color,&pseudo); break;
                    case PIECE_QUEEN: listQueenMoves(pos,i,j,color,&pseudo); break;
                    case PIECE_KING:  listKingMoves(pos,i,j,color,&pseudo); break;
                    default: break;
                }
            }
        }
    }

    // Filtrer les coups qui laissent le roi en échec
    for(int i=0;i<pseudo.count;i++){
        Move m = pseudo.moves[i];
        Cell captured;
        makeMove(pos, m, &captured);
        if(!isKingInCheck(pos,color)) addMoveToList(m.fromX,m.fromY,m.toX,m.toY,m.promotion,movesList);
        undoMove(pos,m,captured);
    }

    // Position finale
    if(movesList->count == 0){
        pos->isFinal = true;
    }
}

bool isThisMoveLegal(Move wantedMove, MoveList *movesList) {
    for (int i = 0; i < movesList->count; i++) {
        if (movesEqual(movesList->moves[i], wantedMove)) {
            return true;
        }
    }
    return false;
}
