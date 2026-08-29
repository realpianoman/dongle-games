#include "Zephyr.h"
#include "Arduino.h"
#include "Display.h"
#include "constants.h"
#include "zephyr.h"

uint32_t Zephyr::getTargetFPS() { return 1; }

void Zephyr::setup() {
    display.setup();

    display.fillScreen(TFT_BLACK);
    display.pushImage(0, 0, ZEPHYR_WIDTH, ZEPHYR_HEIGHT, ZEPHYR_BITS);
}

void Zephyr::update(int frame) {}
