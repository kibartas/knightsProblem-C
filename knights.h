#ifndef KNIGHTS_H
#define KNIGHTS_H

const int knightCount = 4;
const int boardSize = 2;
const int knightMoves[8][2] = { {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {2, -1}, {2, 1}, {1, -2}, {1, 2} };
const wchar_t knight = 0x265E;
const char controlled = 'X';
const int numberOfSpaces = boardSize*boardSize;

void printBoard(const char board[][boardSize][3], int boardNumber);
int canKnightMoveToSpace(const int x, const int y, const int move[]);
void prepareBoards(char board[][boardSize][3], int remove, const int xx, const int yy);
int getAmountOfPossibleMoves(int positions[][9], const int xx, const int yy, char board[][boardSize][3]);
void removeKnight(char board[][boardSize][3], const int x, const int y);
void putFirstKnight(char board[][boardSize][3], int controlledSpaces, int done);
void putKnight(char board[][boardSize][3], int knightNumber, int controlledSpaces, int *done);
void start();

#endif
