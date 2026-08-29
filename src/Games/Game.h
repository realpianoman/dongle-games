#pragma once
#include "Arduino.h"
#include "Display.h"
#include "constants.h"

class Game {
  public:
    virtual ~Game() {};

    Display display;

    virtual uint32_t getTargetFPS() = 0;
    virtual void setup() = 0;

    virtual void update(int frame) = 0;
};
