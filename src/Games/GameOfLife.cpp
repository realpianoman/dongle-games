#include "GameOfLife.h"
#include "Arduino.h"
#include "Display.h"
#include "constants.h"

uint32_t GameOfLife::getTargetFPS() { return 12; }

void GameOfLife::setup() {
    display.setup();

    display.fillScreen(TFT_BLACK);
    reset();
}

void GameOfLife::reset() {
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            grid[r][c] = random(5) == 0;
            drawCell(r, c, grid[r][c]);
        }
    }
}

uint16_t GameOfLife::cellColor(int r, int c) {
    return grid[r][c] ? TFT_WHITE : TFT_BLACK;
}

void GameOfLife::drawCell(int r, int c, bool on) {
    int x = CELL_SIZE * c;
    int y = CELL_SIZE * r;

    display.fillRect(x, y, CELL_SIZE, CELL_SIZE, cellColor(r, c));
}

int GameOfLife::countNeighbors(int r, int c) {
    int count = 0;

    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0)
                continue;

            int nr = (r + dr + ROWS) % ROWS;
            int nc = (c + dc + COLS) % COLS;

            if (grid[nr][nc])
                count++;
        }
    }

    return count;
}

void GameOfLife::updateGrid() {
    bool newGrid[ROWS][COLS]{};

    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            int neighbors = countNeighbors(r, c);

            newGrid[r][c] = grid[r][c];

            if (neighbors < 2)
                newGrid[r][c] = false;
            if (neighbors == 3)
                newGrid[r][c] = true;
            if (neighbors > 3)
                newGrid[r][c] = false;
        }
    }

    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            if (grid[r][c] != newGrid[r][c]) {
                grid[r][c] = newGrid[r][c];

                drawCell(r, c, grid[r][c]);
            }
        }
    }
}

void GameOfLife::update(int frame) {
    updateGrid();

    if (frame % (25 * getTargetFPS()) == 0)
        reset();
}
