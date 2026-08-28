#pragma once
#include <TFT_eSPI.h>

class Display;

class Display {
  public:
    Display();

    void setup();
    void fillScreen(uint16_t color);
    void drawRect(int x, int y, int w, int h, uint16_t color);
    void fillRect(int x, int y, int w, int h, uint16_t color);
    void pixel(int x, int y, uint16_t color);
    void pushImage(int16_t x, int16_t y, int16_t w, int16_t h,
                   const uint16_t *image);

    int16_t width;
    int16_t height;

  private:
    TFT_eSPI tft;
};
