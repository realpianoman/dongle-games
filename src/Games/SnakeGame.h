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
    enum class Cell { Empty, Food, Up, Down, Left, Right };

    const static int CELL_SIZE = 10;

    const static int ROWS = 8;
    const static int COLS = 16;

    Cell grid[ROWS][COLS]{};

    Point head;
    Point tail;

    Point nextPos(Point pos);
    void drawCell(Point pos);
    bool isOppositeDir(Cell d1, Cell d2);
    Cell nextDir();
    Cell dir;

    Point nextFoodPos();
    void generateFood();

    bool running = true;
    bool init = true;
};
