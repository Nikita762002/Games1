#include "snake.h"

void Setup(GameState *state) {
    initscr();
    clear();
    noecho();
    cbreak();
    curs_set(0);

    state->gameOver = false;
    state->dir = STOP;
    state->x = WIDTH / 2;
    state->y = HEIGHT / 2;
    state->fruitX = rand() % WIDTH;
    state->fruitY = rand() % HEIGHT;
    state->score = 0;
    state->nTail = 0;
}

void Draw(const GameState *state) {
    clear();

    for (int i = 0; i < WIDTH + 2; i++)
        mvprintw(0, i, "#");
    mvprintw(1, 0, "#");
    mvprintw(1, WIDTH + 1, "#");

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == state->y && j == state->x)
                mvprintw(i + 1, j + 1, "O");
            else if (i == state->fruitY && j == state->fruitX)
                mvprintw(i + 1, j + 1, "F");
            else {
                bool print = false;
                for (int k = 0; k < state->nTail; k++) {
                    if (state->tailX[k] == j && state->tailY[k] == i) {
                        mvprintw(i + 1, j + 1, "o");
                        print = true;
                    }
                }
                if (!print)
                    mvprintw(i + 1, j + 1, " ");
            }
        }
        mvprintw(i + 1, 0, "#");
        mvprintw(i + 1, WIDTH + 1, "#");
    }

    for (int i = 0; i < WIDTH + 2; i++)
        mvprintw(HEIGHT + 1, i, "#");

    mvprintw(HEIGHT + 2, 0, "Score: %d", state->score);
    refresh();
}

void Input(GameState *state) {
    keypad(stdscr, TRUE);
    halfdelay(1);

    int c = getch();

    switch (c) {
        case KEY_LEFT:
            if (state->dir != RIGHT)
                state->dir = LEFT;
            break;
        case KEY_RIGHT:
            if (state->dir != LEFT)
                state->dir = RIGHT;
            break;
        case KEY_UP:
            if (state->dir != DOWN)
                state->dir = UP;
            break;
        case KEY_DOWN:
            if (state->dir != UP)
                state->dir = DOWN;
            break;
        case 'x':
        case 'X':
        case 'q':
        case 'Q':
            state->gameOver = true;
            break;
    }
}

void Logic(GameState *state) {
    int prevX = state->tailX[0];
    int prevY = state->tailY[0];
    int prev2X, prev2Y;
    state->tailX[0] = state->x;
    state->tailY[0] = state->y;

    for (int i = 1; i < state->nTail; i++) {
        prev2X = state->tailX[i];
        prev2Y = state->tailY[i];
        state->tailX[i] = prevX;
        state->tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (state->dir) {
        case LEFT:
            state->x--;
            break;
        case RIGHT:
            state->x++;
            break;
        case UP:
            state->y--;
            break;
        case DOWN:
            state->y++;
            break;
        default:
            break;
    }

    if (state->x >= WIDTH) state->x = 0; else if (state->x < 0) state->x = WIDTH - 1;
    if (state->y >= HEIGHT) state->y = 0; else if (state->y < 0) state->y = HEIGHT - 1;

    for (int i = 0; i < state->nTail; i++)
        if (state->tailX[i] == state->x && state->tailY[i] == state->y)
            state->gameOver = true;

    if (state->x == state->fruitX && state->y == state->fruitY) {
        state->score += 10;
        state->fruitX = rand() % WIDTH;
        state->fruitY = rand() % HEIGHT;
        state->nTail++;
    }
}

int main() {
    srand(time(NULL));
    GameState state;
    Setup(&state);

    while (!state.gameOver) {
        Draw(&state);
        Input(&state);
        Logic(&state);
        usleep(100000); // Задержка для управления скоростью змейки
    }

    endwin();
    return 0;
}