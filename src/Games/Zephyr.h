#pragma once
#include "Display.h"
#include "Game.h"
#include "constants.h"

class Zephyr : public Game {
  public:
    uint32_t getTargetFPS() override;
    void setup() override;
    void update(int frame) override;

  private:
    Display display = Display();
};
