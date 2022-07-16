#include "Chip8Screen.h"
#include "CustomException.h"
#include "constants.h"
#include <cstdint>

static void screenCheckBounds(int &x, int &y) {

  if (x < 0 && x >= chip8Width && y < 0 && y >= chip8Height) {
    throw CustomException(const_cast<char *>("screen point out of bounds "));
  }
}

void Chip8Screen::chip8ScreenSet(int x, int y) {

  screenCheckBounds(x, y);

  pixels[y][x] = true;
}

bool Chip8Screen::chip8ScreenIsSet(int x, int y) {
  screenCheckBounds(x, y);
  return pixels[y][x];
}

void Chip8Screen::print() {}
Chip8Screen::Chip8Screen() {
  for (int x{}; x < chip8Width; x++) {
    for (int y{}; y < chip8Height; y++) {
    }
  }
}
bool Chip8Screen::drawSprite(int x, int y, const char *sprite, int num) {

  bool pixelCollision = false;

  for (uint8_t ly = 0; ly < num; ly++) {
    uint8_t c = sprite[ly];
    for (uint8_t lx = 0; lx < 8; lx++) {
      if ((c & (0b10000000 >> lx)) == 0) {
        continue;
      }
      if (pixels.at((ly + y) % chip8Height).at((lx + x) % chip8Width)) {
        pixelCollision = true;
      }
      pixels.at((ly + y) % chip8Height).at((lx + x) % chip8Width) ^= true;
    }
  }

  return pixelCollision;
}

void Chip8Screen::chip8screenClear() {

  for (size_t i{}; i < pixels.size(); i++) {
    for (size_t j{}; j < pixels[i].size(); j++) {
      pixels[i][j] = 0;
    }
  }
}
