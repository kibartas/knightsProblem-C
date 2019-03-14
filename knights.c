#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include "knights.h"

void printBoard(const char board[][boardSize][3], int boardNumber) {
	setlocale(LC_ALL, "");
	for (int y = 0; y < boardSize; y++) {
		for (int x = 0; x < boardSize; x++) {
			if (boardNumber == 0) {
				printf(" %d ", board[x][y][boardNumber]);
			} else if (boardNumber == 1) {
				if (board[x][y][boardNumber] != 0) {
					printf(" %lc ", knight);
				} else {
					printf(" - ");
				}
			} else if (boardNumber == 2) {
				if (board[x][y][boardNumber] != 0) {
					printf(" %c ", controlled);
				} else {
					printf(" - ");
				}
			} else {
				printf("Unknown board number\n\n");
				return;
			}
		}
		printf("\n");
	}
	printf("\n\n");
}

// returns 1 if a knight can knightMoves to a selected space, 0 if otherwise
int canKnightMoveToSpace(const int x, const int y, const int knightMoves[]) {
	if (x+knightMoves[0] >= 0 && y+knightMoves[1] >= 0 && x+knightMoves[0] < boardSize && y+knightMoves[1] < boardSize) {
		return 1;
	} else {
		return 0;
	}
}

// if reknightMoves is 1 the first part of the function acts like a knight is being reknightMovesd, if 0 - otherwise
void prepareBoards(char board[][boardSize][3], int reknightMoves, const int xx, const int yy) {
	if (reknightMoves == 0) {
		board[xx][yy][2]++;
		for (int a = 0; a < 8; a++) {
			if (canKnightMoveToSpace(xx, yy, knightMoves[a]) == 1) {
				board[xx+knightMoves[a][0]][yy+knightMoves[a][1]][2]++;
			}
		}
	} else if (reknightMoves == 1) {
		board[xx][yy][2]--;
		for (int a = 0; a < 8; a++) {
			if (canKnightMoveToSpace(xx, yy, knightMoves[a]) == 1) {
				board[xx+knightMoves[a][0]][yy+knightMoves[a][1]][2]--;
			}
		}
	}
	for (int y = 0; y < boardSize; y++) {
		for (int x = 0; x < boardSize; x++) {
			board[x][y][0] = 0;
			if (board[x][y][1] == knight) {
				continue;
			}
			for (int a = 0; a < 8; a++) {
				if (canKnightMoveToSpace(x, y, knightMoves[a]) == 1 && board[x+knightMoves[a][0]][y+knightMoves[a][1]][2] == 0) {
					board[x][y][0]++;			
				}
			}
			if (board[x][y][2] != 0) {
				board[x][y][0]--;	
			}
			if (board[x][y][1] != knight) {
				board[x][y][0]++;	
			}
		}
	}
}

int getAmountOfPossibleMoves(int positions[][9], const int xx, const int yy, char board[][boardSize][3]) {
	positions[0][0] = xx;
	positions[1][0] = yy;
	int counter = 1;
	for (int a = 0; a < 8; a++) {
		if (canKnightMoveToSpace(xx, yy, knightMoves[a]) == 1 && board[xx+knightMoves[a][0]][yy+knightMoves[a][1]][1] != knight) {
			positions[0][counter] = xx+knightMoves[a][0];
			positions[1][counter++] = yy+knightMoves[a][1];
		}
	}
	return counter;
}

void removeKnight(char board[][boardSize][3], const int x, const int y) {
	board[x][y][1] = 0;
	prepareBoards(board, 1, x, y);
}

void putFirstKnight(char board[][boardSize][3], int controlledSpaces, int done) {
	int positions[2][9];
	int possibilities = getAmountOfPossibleMoves(positions, 0, 0, board);
	int controlledSpacesBuffer = 0;
	for (int i = 0; i < possibilities; i++) {
		board[positions[0][i]][positions[1][i]][1] = knight;
		controlledSpacesBuffer = board[positions[0][i]][positions[1][i]][0];
		if (controlledSpaces+controlledSpacesBuffer >= numberOfSpaces) {
			printBoard(board, 1);
			return;
		}
		prepareBoards(board, 0, positions[0][i], positions[1][i]);
		putKnight(board, 2, controlledSpaces+controlledSpacesBuffer, &done);
		if (done == 1) {
			return;
		}
		removeKnight(board, positions[0][i], positions[1][i]);
	}
	printf("Layout not found\n");
}


void putKnight(char board[][boardSize][3], int knightNumber, int controlledSpaces, int *done) {
	int positions[2][9];
	int minXX;
	int minYY;
	int maxXX;
	int maxYY;
	int lowestValue = 10;
	int highestValue = 0;
	for (int y = 0; y < boardSize; y++) {
		for (int x = 0; x < boardSize; x++) {
				if (board[x][y][0] < lowestValue && board[x][y][1] != knight && board[x][y][2] == 0) {
					lowestValue = board[x][y][0];
					minXX = x;
					minYY = y;
				}
				if (board[x][y][0] > highestValue) {
					highestValue = board[x][y][0];	
					maxXX = x;
					maxYY = y;
				}
		}
	}
	if ((knightCount+1-knightNumber)*highestValue+controlledSpaces < numberOfSpaces) {
		return;
	}
	if (knightNumber == knightCount) {
		if (controlledSpaces+highestValue >= numberOfSpaces) {
			board[maxXX][maxYY][1] = knight;
			printBoard(board, 1);
			*done = 1;
			return;
		}
	}
	int possibilities = getAmountOfPossibleMoves(positions, minXX, minYY, board);
	int controlledSpacesBuffer = 0;
	for (int i = 0; i < possibilities; i++) {
		board[positions[0][i]][positions[1][i]][1] = knight;
		controlledSpacesBuffer = board[positions[0][i]][positions[1][i]][0];
		prepareBoards(board, 0, positions[0][i], positions[1][i]);
		if (knightNumber != knightCount) {
			putKnight(board, knightNumber+1, controlledSpaces+controlledSpacesBuffer, done);
			if (*done == 1) {
				return;
			}
		}
		removeKnight(board, positions[0][i], positions[1][i]);
	}
}

void start() {
	char board[boardSize][boardSize][3];
	for (int y = 0; y < boardSize; y++) {
		for (int x = 0; x < boardSize; x++) {
			board[x][y][0] = 0;
			board[x][y][1] = 0;
			board[x][y][2] = 0;
		}
	}
	prepareBoards(board, 2, 0, 0);
	if (knightCount >= 1) {
		putFirstKnight(board, 0, 0);
	} else {
		printf("Layout not found\n");
	}
}

int main() {
	start();
}

