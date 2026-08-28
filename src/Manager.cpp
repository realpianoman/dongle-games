#include "Manager.h"
#include "Arduino.h"
#include "Display.h"
#include "TFT_eSPI.h"

Manager::Manager() { display = Display(); }

void Manager::setup() { display.setup(); }

void Manager::update(int frame) {
    display.fillScreen(TFT_BLACK);

    display.pixel(static_cast<int>(display.width * (frame % 24) / 24.0),
                  display.height / 2, TFT_RED);
}
