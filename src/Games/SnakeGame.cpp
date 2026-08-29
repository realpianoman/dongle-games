#include "SnakeGame.h"
#include "Arduino.h"
#include "Display.h"
#include "constants.h"

uint32_t SnakeGame::getTargetFPS() { return 2; }

void SnakeGame::setup() {
    if (init)
        display.setup();

    init = false;
    running = true;

    display.fillScreen(TFT_BLACK);

    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            grid[r][c] = Cell::Empty;
        }
    }

    head.r = ROWS / 2;
    head.c = 2;

    tail.r = head.r;
    tail.c = head.c;

    grid[head.r][head.c] = Cell::Right;
    dir = Cell::Right;

    generateFood();
}

Point SnakeGame::nextPos(Point pos) {
    Point next;

    next.r = pos.r;
    next.c = pos.c;

    switch (grid[pos.r][pos.c]) {
    case Cell::Up:
        next.r--;
        break;
    case Cell::Down:
        next.r++;
        break;
    case Cell::Left:
        next.c--;
        break;
    case Cell::Right:
        next.c++;
        break;
    default:
        break;
    }

    if (next.r < 0 || next.c < 0 || next.r >= ROWS || next.c >= COLS) {
        // OOB
        next.r = -1;
        next.c = -1;
    }

    return next;
}

void SnakeGame::drawCell(Point pos) {
    int x = CELL_SIZE * pos.c;
    int y = CELL_SIZE * pos.r;

    Cell cell = grid[pos.r][pos.c];

    uint16_t color = TFT_BLACK;
    if (cell == Cell::Empty)
        color = TFT_BLACK;
    else if (cell == Cell::Food)
        color = TFT_RED;
    else
        color = TFT_GREEN;

    display.fillRect(x, y, CELL_SIZE, CELL_SIZE, color);
}

bool SnakeGame::isOppositeDir(Cell d1, Cell d2) {
    if (d1 == Cell::Up && d2 == Cell::Down)
        return true;
    if (d1 == Cell::Left && d2 == Cell::Right)
        return true;

    return false;
}

SnakeGame::Cell SnakeGame::nextDir() {
    Cell options[] = {Cell::Up, Cell::Down, Cell::Right, Cell::Left};

    Cell newDir = options[random(4)];
    if (isOppositeDir(dir, newDir) || isOppositeDir(newDir, dir)) {
        newDir = nextDir();
    }

    return newDir;
}

Point SnakeGame::nextFoodPos() {
    int r = random(ROWS);
    int c = random(COLS);

    if (grid[r][c] != Cell::Empty)
        return nextFoodPos();

    Point out;
    out.r = r;
    out.c = c;
    return out;
}

void SnakeGame::generateFood() {
    Point foodPos = nextFoodPos();

    grid[foodPos.r][foodPos.c] = Cell::Food;

    drawCell(foodPos);
}

void SnakeGame::update(int frame) {
    if (!running) {
        setup();
    }

    dir = nextDir();

    Point nextHead = nextPos(head);
    if (nextHead.r == -1) {
        running = false;
        return;
    }

    if (grid[nextHead.r][nextHead.c] != Cell::Empty &&
        grid[nextHead.r][nextHead.c] != Cell::Food) {
        running = false;
        return;
    }

    bool skipTail = false;
    if (grid[nextHead.r][nextHead.c] == Cell::Food) {
        skipTail = true;

        generateFood();
    }

    grid[nextHead.r][nextHead.c] = dir;
    head.r = nextHead.r;
    head.c = nextHead.c;

    if (!skipTail) {
        Point nextTail = nextPos(tail);
        grid[tail.r][tail.c] = Cell::Empty;

        drawCell(tail);

        tail.r = nextTail.r;
        tail.c = nextTail.c;
    }

    drawCell(head);
}
