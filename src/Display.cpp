#include <Display.h>
#include <TFT_eSPI.h>

Display::Display() {}

void Display::setup() {
    tft.init();
    tft.setRotation(1);

    tft.fillScreen(TFT_BLUE);

    width = tft.width();
    height = tft.height();
}

void Display::fillScreen(uint16_t color) { tft.fillScreen(color); }

void Display::drawRect(int x, int y, int w, int h, uint16_t color) {
    tft.drawRect(x, y, w, h, color);
}

void Display::fillRect(int x, int y, int w, int h, uint16_t color) {
    tft.fillRect(x, y, w, h, color);
}

void Display::pixel(int x, int y, uint16_t color) {
    drawRect(x, y, 1, 1, color);
}
