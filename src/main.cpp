#include "Display.h"
#include "Manager.h"
#include "constants.h"
#include <Arduino.h>

uint32_t lastFrame = 0;
uint32_t fpsTimer = 0;
uint32_t frameCount = 0;
int frame = 0;

Manager manager = Manager();

void setup() {
    Serial.begin(115200);
    delay(500);

    lastFrame = millis();
    fpsTimer = millis();

    manager.setup();
}

void loop() {
    uint32_t now = millis();

    if (now - lastFrame < constants::TARGET_FRAME_TIME)
        return;
    lastFrame += constants::TARGET_FRAME_TIME;
    frame++;

    manager.update(frame);

    frameCount++;

    if (now - fpsTimer >= 1000) {
        // Serial.printf("FPS: %lu\n", frameCount);

        frameCount = 0;
        fpsTimer = now;
    }
}
