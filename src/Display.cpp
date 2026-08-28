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

void Display::pushImage(int16_t x, int16_t y, int16_t w, int16_t h,
                        const uint16_t *image) {
    for (int16_t row = 0; row < h; row++) {
        for (int16_t col = 0; col < w; col++) {
            tft.drawPixel(x + col, y + row, image[row * w + col]);
        }
    }
}

void Display::pixel(int x, int y, uint16_t color) {
    tft.drawPixel(x, y, color);
}
