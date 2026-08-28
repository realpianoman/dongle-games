#pragma once
#include "Display.h"
#include "constants.h"

class Zephyr;

class Zephyr {
  public:
    Zephyr();

    void setup();
    void update(int frame);

  private:
    Display display = Display();
};
