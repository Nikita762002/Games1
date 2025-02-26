#ifndef SNAKE_H
#define SNAKE_H

#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define WIDTH 40
#define HEIGHT 20

enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };

typedef struct {
    int x, y, fruitX, fruitY, score;
    int tailX[100], tailY[100];
    int nTail;
    enum eDirection dir;
    bool gameOver;
} GameState;

void Setup(GameState *state);
void Draw(const GameState *state);
void Input(GameState *state);
void Logic(GameState *state);

#endif 
