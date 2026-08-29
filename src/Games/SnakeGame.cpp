// Highkey slopped, I had working snake but I'm lazy for the automation

#include "SnakeGame.h"
#include "Arduino.h"
#include "Display.h"
#include "constants.h"

uint32_t SnakeGame::getTargetFPS() { return 6; }

void SnakeGame::setup() {
    if (init) {
        display.setup();
        init = false;
    }

    running = true;

    display.fillScreen(TFT_BLACK);

    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            grid[r][c] = Cell::Empty;
        }
    }

    length = 3;

    int startRow = ROWS / 2;
    int startCol = 4;

    body[0] = {startRow, startCol};
    body[1] = {startRow, startCol - 1};
    body[2] = {startRow, startCol - 2};

    head = body[0];
    tail = body[length - 1];

    dir = Direction::Right;

    for (int i = 0; i < length; i++) {
        drawCell(body[i]);
    }

    generateFood();
}

bool SnakeGame::inBounds(int r, int c) {
    return r >= 0 && r < ROWS && c >= 0 && c < COLS;
}

bool SnakeGame::samePoint(Point a, Point b) { return a.r == b.r && a.c == b.c; }

bool SnakeGame::snakeContains(Point p) { return snakeContains(p, false); }

bool SnakeGame::snakeContains(Point p, bool ignoreTail) {
    int count = length;

    if (ignoreTail && count > 0) {
        count--;
    }

    for (int i = 0; i < count; i++) {
        if (samePoint(body[i], p)) {
            return true;
        }
    }

    return false;
}

Point SnakeGame::movePoint(Point p, Direction direction) {
    Point result = p;

    switch (direction) {
    case Direction::Up:
        result.r--;
        break;

    case Direction::Down:
        result.r++;
        break;

    case Direction::Left:
        result.c--;
        break;

    case Direction::Right:
        result.c++;
        break;
    }

    return result;
}

Point SnakeGame::bfsNext() {
    bool visited[ROWS][COLS] = {};
    Point parent[ROWS][COLS];

    Point queue[MAX_SNAKE_LENGTH];

    int front = 0;
    int back = 0;

    queue[back++] = head;
    visited[head.r][head.c] = true;

    Direction directions[4];

    switch (dir) {
    case Direction::Up:
        directions[0] = Direction::Up;
        directions[1] = Direction::Right;
        directions[2] = Direction::Left;
        directions[3] = Direction::Down;
        break;

    case Direction::Down:
        directions[0] = Direction::Down;
        directions[1] = Direction::Right;
        directions[2] = Direction::Left;
        directions[3] = Direction::Up;
        break;

    case Direction::Left:
        directions[0] = Direction::Left;
        directions[1] = Direction::Up;
        directions[2] = Direction::Down;
        directions[3] = Direction::Right;
        break;

    case Direction::Right:
        directions[0] = Direction::Right;
        directions[1] = Direction::Down;
        directions[2] = Direction::Up;
        directions[3] = Direction::Left;
        break;
    }

    while (front < back) {
        Point current = queue[front++];

        if (grid[current.r][current.c] == Cell::Food) {
            Point path = current;
            while (!samePoint(parent[path.r][path.c], head)) {
                path = parent[path.r][path.c];
            }

            return path;
        }

        for (int d = 0; d < 4; d++) {
            Point next = movePoint(current, directions[d]);

            if (!inBounds(next.r, next.c)) {
                continue;
            }

            if (visited[next.r][next.c]) {
                continue;
            }

            if (grid[next.r][next.c] == Cell::Food) {
                visited[next.r][next.c] = true;
                parent[next.r][next.c] = current;
                queue[back++] = next;
                continue;
            }

            if (grid[next.r][next.c] == Cell::Empty) {
                if (samePoint(next, tail)) {
                    visited[next.r][next.c] = true;
                    parent[next.r][next.c] = current;
                    queue[back++] = next;
                    continue;
                }

                if (snakeContains(next)) {
                    continue;
                }

                visited[next.r][next.c] = true;
                parent[next.r][next.c] = current;
                queue[back++] = next;
            }
        }
    }

    return head;
}

void SnakeGame::drawCell(Point pos) {
    if (!inBounds(pos.r, pos.c)) {
        return;
    }

    int x = CELL_SIZE * pos.c;
    int y = CELL_SIZE * pos.r;

    uint16_t color = TFT_BLACK;

    if (grid[pos.r][pos.c] == Cell::Food) {
        color = TFT_RED;
    } else if (snakeContains(pos)) {
        color = TFT_GREEN;
    }

    display.fillRect(x, y, CELL_SIZE, CELL_SIZE, color);
}

void SnakeGame::eraseCell(Point pos) {
    if (!inBounds(pos.r, pos.c)) {
        return;
    }

    int x = CELL_SIZE * pos.c;
    int y = CELL_SIZE * pos.r;

    display.fillRect(x, y, CELL_SIZE, CELL_SIZE, TFT_BLACK);
}

Point SnakeGame::nextFoodPos() {
    Point available[MAX_SNAKE_LENGTH];

    int count = 0;

    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            Point p = {r, c};

            if (grid[r][c] != Cell::Empty) {
                continue;
            }

            if (snakeContains(p)) {
                continue;
            }

            available[count++] = p;
        }
    }

    if (count == 0) {
        return {-1, -1};
    }

    return available[random(count)];
}

void SnakeGame::generateFood() {
    Point food = nextFoodPos();

    if (!inBounds(food.r, food.c)) {
        running = false;
        return;
    }

    grid[food.r][food.c] = Cell::Food;

    drawCell(food);
}

void SnakeGame::update(int frame) {
    if (!running) {
        setup();
        return;
    }

    Point nextHead = bfsNext();

    if (samePoint(nextHead, head)) {
        running = false;
        return;
    }

    if (nextHead.r < head.r) {
        dir = Direction::Up;
    } else if (nextHead.r > head.r) {
        dir = Direction::Down;
    } else if (nextHead.c < head.c) {
        dir = Direction::Left;
    } else if (nextHead.c > head.c) {
        dir = Direction::Right;
    }

    bool eatingFood = grid[nextHead.r][nextHead.c] == Cell::Food;

    if (snakeContains(nextHead, !eatingFood)) {
        running = false;
        return;
    }

    if (!eatingFood) {
        tail = body[length - 1];

        eraseCell(tail);
    }

    int newLength = eatingFood ? length + 1 : length;

    if (newLength > MAX_SNAKE_LENGTH) {
        newLength = MAX_SNAKE_LENGTH;
    }

    for (int i = newLength - 1; i > 0; i--) {
        body[i] = body[i - 1];
    }

    body[0] = nextHead;

    length = newLength;

    head = body[0];
    tail = body[length - 1];

    if (eatingFood) {
        grid[nextHead.r][nextHead.c] = Cell::Empty;
    }

    drawCell(head);

    if (eatingFood) {
        generateFood();
    }
}
