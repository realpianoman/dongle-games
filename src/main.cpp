#include "./Games/Game.h"
#include "./Games/GameOfLife.h"
#include "./Games/Zephyr.h"
#include "Display.h"
#include "constants.h"
#include <Arduino.h>

uint32_t lastFrame = 0;
uint32_t fpsTimer = 0;
uint32_t frameCount = 0;
int frame = 0;

GameOfLife gol;
Zephyr zephyr;
Game *currentGame = &gol;

uint32_t TARGET_FPS;
uint32_t TARGET_FRAME_TIME;

void setup() {
    Serial.begin(115200);
    delay(250);

    TARGET_FPS = currentGame->getTargetFPS();
    TARGET_FRAME_TIME = 1000 / TARGET_FPS;

    lastFrame = millis();
    fpsTimer = millis();

    currentGame->setup();
}

void loop() {
    uint32_t now = millis();

    if (now - lastFrame < TARGET_FRAME_TIME)
        return;
    lastFrame += TARGET_FRAME_TIME;
    frame++;

    currentGame->update(frame);

    frameCount++;

    if (now - fpsTimer >= 1000) {
        Serial.printf("FPS: %lu\n", frameCount);

        frameCount = 0;
        fpsTimer = now;
    }
}
