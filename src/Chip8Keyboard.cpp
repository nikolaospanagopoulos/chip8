#include "Chip8Keyboard.h"

void Chip8Keyboard::key_down(int key) { keyboard[key] = true; }
void Chip8Keyboard::key_up(int key) { keyboard[key] = false; }
bool Chip8Keyboard::key_is_down(int key) { return keyboard[key]; };
int Chip8Keyboard::mapKeyboard(char &key) {

  for (size_t i{}; i < 16; i++) {
    if (keyboardMap[i] == key) {
      return i;
    }
  }
  return -1;
}

char Chip8Keyboard::waitForKeyPress() {
  SDL_Event event;
  while (SDL_WaitEvent(&event)) {
    if (event.type != SDL_KEYDOWN) {
      continue;
    }

    char c = event.key.keysym.sym;
    char chip8key = mapKeyboard(c);
    if (chip8key != -1) {
      return chip8key;
    }
  }
  return -1;
}
