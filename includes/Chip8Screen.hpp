#pragma once

#include "constants.h"
#include <array>
#include <cstdint>
class Chip8Screen {

  std::array<std::array<bool, CHIP8_WIDTH>, CHIP8_HEIGHT> *pixels;
  void checkScreenBounds(int x, int y) const;

public:
  void screenClear();
  void screenSet(int x, int y);
  bool drawSprite(int x, int y, const uint8_t *sprite, int numOfBytes);
  bool screenIsSet(int x, int y) const;
  Chip8Screen();
  ~Chip8Screen();
};
