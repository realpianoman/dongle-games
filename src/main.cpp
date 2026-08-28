#include "Display.h"
#include "GameOfLife.h"
#include "Zephyr.h"
#include "constants.h"
#include <Arduino.h>

uint32_t lastFrame = 0;
uint32_t fpsTimer = 0;
uint32_t frameCount = 0;
int frame = 0;

int active = 0;

GameOfLife gol = GameOfLife();
Zephyr zephyr = Zephyr();

void setup() {
    Serial.begin(115200);
    delay(250);

    lastFrame = millis();
    fpsTimer = millis();

    bool zeph = random(5) == 0;
    if (zeph)
        active = 1;

    if (active == 0)
        gol.setup();
    if (active == 1)
        zephyr.setup();
}

void loop() {
    uint32_t now = millis();

    if (now - lastFrame < constants::TARGET_FRAME_TIME)
        return;
    lastFrame += constants::TARGET_FRAME_TIME;
    frame++;

    if (frame > constants::TARGET_FPS)
        if (active == 0)
            gol.update(frame - constants::TARGET_FPS);

    frameCount++;

    if (now - fpsTimer >= 1000) {
        Serial.printf("FPS: %lu\n", frameCount);

        frameCount = 0;
        fpsTimer = now;
    }
}
