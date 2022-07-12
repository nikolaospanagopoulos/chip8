#pragma once
#include "constants.h"
#include <array>
#include <vector>
class Chip8Screen {

public:
  std::array<std::array<bool, 64>, 32> pixels{};
  void chip8ScreenSet(int x, int y);
  bool chip8ScreenIsSet(int x, int y);
  bool drawSprite(int x, int y, const unsigned int *sprite, int num);
  void print();

  void chip8screenClear();
  Chip8Screen();
};
