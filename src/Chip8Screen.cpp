#include "../includes/Chip8Screen.hpp"
#include "../includes/CustomException.hpp"
#include <cstring>
#include <memory.h>

Chip8Screen::Chip8Screen() {
  pixels = new std::array<std::array<bool, CHIP8_WIDTH>, CHIP8_HEIGHT>{};
}

Chip8Screen::~Chip8Screen()

{
  delete pixels;
}

void Chip8Screen::checkScreenBounds(int x, int y) const {
  if (x < 0 || x > CHIP8_WIDTH || y < 0 || y > CHIP8_HEIGHT) {
    throw CustomException((char *)("not correct dimentions"));
  }
}

void Chip8Screen::screenSet(int x, int y)

{
  checkScreenBounds(x, y);
  (*pixels)[y][x] = true;
}

bool Chip8Screen::screenIsSet(int x, int y) const {
  checkScreenBounds(x, y);
  return pixels->at(y).at(x);
}

bool Chip8Screen::drawSprite(int x, int y, const uint8_t *sprite,
                             int numOfBytes) {

  bool pixelCollition = false;
  for (int ly{}; ly < numOfBytes; ly++) {
    char c = sprite[ly];
    for (int lx{}; lx < 8; lx++) {
      if ((c & (0b10000000 >> lx)) == 0) {
        continue;
      }
      // check later
      if (screenIsSet((lx + x) % CHIP8_WIDTH, (ly + y) % CHIP8_HEIGHT)) {
        pixelCollition = true;
      }
      (*pixels)[(ly + y) % CHIP8_HEIGHT][(lx + x) % CHIP8_WIDTH] ^= true;
    }
  }

  return pixelCollition;
}

void Chip8Screen::screenClear() { std::memset(pixels, 0, sizeof(*pixels)); }
