#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define WIDTH 40
#define HEIGHT 20

int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
enum eDirection dir;

bool gameOver;

void Setup() {
    initscr();
    clear();
    noecho();
    cbreak();
    curs_set(0);

    gameOver = false;
    dir = STOP;
    x = WIDTH / 2;
    y = HEIGHT / 2;
    fruitX = rand() % WIDTH;
    fruitY = rand() % HEIGHT;
    score = 0;
}

void Draw() {
    clear();

    for (int i = 0; i < WIDTH + 2; i++)
        mvprintw(0, i, "#");
    mvprintw(1, 0, "#");
    mvprintw(1, WIDTH + 1, "#");

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == y && j == x)
                mvprintw(i + 1, j + 1, "O");
            else if (i == fruitY && j == fruitX)
                mvprintw(i + 1, j + 1, "F");
            else {
                bool print = false;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
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

    mvprintw(HEIGHT + 2, 0, "Score: %d", score);
    refresh();
}

void Input() {
    keypad(stdscr, TRUE);
    halfdelay(1);

    int c = getch();

    switch (c) {
        case KEY_LEFT:
            if (dir != RIGHT)
                dir = LEFT;
            break;
        case KEY_RIGHT:
            if (dir != LEFT)
                dir = RIGHT;
            break;
        case KEY_UP:
            if (dir != DOWN)
                dir = UP;
            break;
        case KEY_DOWN:
            if (dir != UP)
                dir = DOWN;
            break;
        case 'x':
            case 'X':
            case 'q':
            case 'Q':
                gameOver = true;
                break;
    }
}

void Logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;

    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir) {
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        default:
            break;
    }

    if (x >= WIDTH) x = 0; else if (x < 0) x = WIDTH - 1;
    if (y >= HEIGHT) y = 0; else if (y < 0) y = HEIGHT - 1;

    for (int i = 0; i < nTail; i++)
        if (tailX[i] == x && tailY[i] == y)
            gameOver = true;

    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % WIDTH;
        fruitY = rand() % HEIGHT;
        nTail++;
    }
}

int main() {
    srand(time(NULL));
    Setup();

    while (!gameOver) {
        Draw();
        Input();
        Logic();
        usleep(100000); // Задержка для управления скоростью змейки
    }

    endwin();
    return 0;
}
