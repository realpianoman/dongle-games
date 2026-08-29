#include "./Games/Game.h"
#include "./Games/GameOfLife.h"
#include "./Games/SnakeGame.h"
#include "./Games/Zephyr.h"
#include "Display.h"
#include "constants.h"
#include <Arduino.h>
#include <Preferences.h>

Preferences preferences;

uint32_t lastFrame = 0;
uint32_t fpsTimer = 0;
uint32_t frameCount = 0;
int frame = 0;

GameOfLife gol;
SnakeGame snakeGame;
Zephyr zephyr;

Game *currentGame = nullptr;

uint32_t TARGET_FPS;
uint32_t TARGET_FRAME_TIME;

// Slop function, I need to look into esp32 preferences, this is temp
void chooseNextGame() {
    preferences.begin("games", false);

    int gameIndex = preferences.getInt("game", 0);

    if (gameIndex == 0) {
        currentGame = &snakeGame;
    } else if (gameIndex == 1) {
        currentGame = &gol;
    } else {
        currentGame = &zephyr;
    }

    preferences.putInt("game", (gameIndex + 1) % 3);

    preferences.end();
}

void setup() {
    Serial.begin(115200);
    delay(250);

    chooseNextGame();

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
