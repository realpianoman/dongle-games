#pragma once
#include "Display.h"
#include "constants.h"

class GameOfLife;

class GameOfLife {
  public:
    GameOfLife();

    void setup();
    void update(int frame);

  private:
    void reset();

    void drawCell(int r, int c, bool on);
    bool getCell(int r, int c);
    int countNeighbors(int r, int c);
    void updateGrid();

    Display display = Display();
    bool grid[constants::ROWS][constants::COLS]{};
};
