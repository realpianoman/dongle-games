#pragma once
#include "Display.h"
#include "Game.h"
#include "constants.h"

class GameOfLife : public Game {
  public:
    uint32_t getTargetFPS() override;
    void setup() override;
    void update(int frame) override;

  private:
    void reset();

    void drawCell(int r, int c, bool on);
    uint16_t cellColor(int r, int c);
    uint16_t colorGradient(uint16_t c1, uint16_t c2, uint8_t amt);
    bool getCell(int r, int c);
    int countNeighbors(int r, int c);
    void updateGrid();

    const static int CELL_SIZE = 2;

    const static int ROWS = 40;
    const static int COLS = 80;

    bool grid[ROWS][COLS]{};
};
