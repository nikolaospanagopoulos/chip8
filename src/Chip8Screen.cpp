#include "Chip8Screen.h"
#include "CustomException.h"
#include "constants.h"

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
  std::cout << "lalal";
  for (int x{}; x < chip8Width; x++) {
    for (int y{}; y < chip8Height; y++) {
    }
  }
}
