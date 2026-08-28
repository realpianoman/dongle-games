#include "Manager.h"
#include "Arduino.h"
#include "Display.h"
#include "TFT_eSPI.h"
#include "constants.h"

Manager::Manager() {}

void Manager::setup() {
    display.setup();

    display.fillScreen(TFT_BLACK);
    reset();

    delay(1000);
}

void Manager::reset() {
    for (int r = 0; r < constants::ROWS; r++) {
        for (int c = 0; c < constants::COLS; c++) {
            grid[r][c] = random(5) == 0;
            drawCell(r, c, grid[r][c]);
        }
    }
}

void Manager::drawCell(int r, int c, bool on) {
    int x = constants::CELL_SIZE * c;
    int y = constants::CELL_SIZE * r;

    display.fillRect(x, y, constants::CELL_SIZE, constants::CELL_SIZE,
                     on ? TFT_WHITE : TFT_BLACK);
}

int Manager::countNeighbors(int r, int c) {
    int count = 0;

    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0)
                continue;

            int nr = (r + dr + constants::ROWS) % constants::ROWS;
            int nc = (c + dc + constants::COLS) % constants::COLS;

            if (grid[nr][nc])
                count++;
        }
    }

    return count;
}

void Manager::updateGrid() {
    bool newGrid[constants::ROWS][constants::COLS]{};

    for (int r = 0; r < constants::ROWS; r++) {
        for (int c = 0; c < constants::COLS; c++) {
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

    for (int r = 0; r < constants::ROWS; r++) {
        for (int c = 0; c < constants::COLS; c++) {
            if (grid[r][c] != newGrid[r][c]) {
                grid[r][c] = newGrid[r][c];

                drawCell(r, c, grid[r][c]);
            }
        }
    }
}

void Manager::update(int frame) {
    updateGrid();

    if (frame % (25 * constants::TARGET_FPS) == 0)
        reset();
}
