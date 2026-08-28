#pragma once
#include "Display.h"

class Manager;

class Manager {
  public:
    Manager();

    void setup();
    void update(int frame);

  private:
    Display display;
};
