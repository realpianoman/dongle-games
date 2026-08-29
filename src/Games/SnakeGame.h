#pragma once

#include "Display.h"
#include "Game.h"
#include "constants.h"

struct Point {
    int r;
    int c;
};

class SnakeGame : public Game {
  public:
    uint32_t getTargetFPS() override;
    void setup() override;
    void update(int frame) override;

  private:
    enum class Cell { Empty, Food };

    enum class Direction { Up, Down, Left, Right };

    static const int CELL_SIZE = 10;

    static const int ROWS = 8;
    static const int COLS = 16;
    static const int MAX_SNAKE_LENGTH = ROWS * COLS;

    Cell grid[ROWS][COLS]{};

    Point body[MAX_SNAKE_LENGTH]{};

    int length = 0;

    Point head{};
    Point tail{};

    Direction dir = Direction::Right;

    bool running = true;
    bool init = true;

    bool inBounds(int r, int c);

    bool samePoint(Point a, Point b);

    bool snakeContains(Point p);
    bool snakeContains(Point p, bool ignoreTail);

    Point bfsNext();

    Point movePoint(Point p, Direction direction);

    void drawCell(Point pos);
    void eraseCell(Point pos);

    Point nextFoodPos();
    void generateFood();
};
