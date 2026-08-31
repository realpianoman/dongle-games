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

uint16_t GameOfLife::colorGradient(uint16_t c1, uint16_t c2, uint8_t amt) {
    uint8_t r1 = (c1 >> 11) & 0x1F;
    uint8_t g1 = (c1 >> 5) & 0x3F;
    uint8_t b1 = c1 & 0x1F;

    uint8_t r2 = (c2 >> 11) & 0x1F;
    uint8_t g2 = (c2 >> 5) & 0x3F;
    uint8_t b2 = c2 & 0x1F;

    uint8_t r = r1 + ((r2 - r1) * amt) / 255;
    uint8_t g = g1 + ((g2 - g1) * amt) / 255;
    uint8_t b = b1 + ((b2 - b1) * amt) / 255;

    return (r << 11) | (g << 5) | b;
}

uint16_t GameOfLife::cellColor(int r, int c) {
    return grid[r][c] ? colorGradient(TFT_BLUE, TFT_MAGENTA,
                                      255 * (countNeighbors(r, c) / 4.0))
                      : TFT_BLACK;
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
            // if (grid[r][c] != newGrid[r][c]) {
            grid[r][c] = newGrid[r][c];

            drawCell(r, c, grid[r][c]);
            // }
        }
    }
}

void GameOfLife::update(int frame) {
    updateGrid();

    if (frame % (25 * getTargetFPS()) == 0)
        reset();
}
