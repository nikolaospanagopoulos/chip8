#include "../includes/Chip8Keyboard.hpp"
#include <SDL2/SDL_keycode.h>
#include <cstddef>

Chip8Keyboard::Chip8Keyboard() {
  keyboard = new bool[16];

  keyboardPC = new char[16]{SDLK_0, SDLK_1, SDLK_2, SDLK_3, SDLK_4, SDLK_5,
                            SDLK_6, SDLK_7, SDLK_8, SDLK_9, SDLK_a, SDLK_b,
                            SDLK_c, SDLK_d, SDLK_e, SDLK_f

  };
}
Chip8Keyboard::~Chip8Keyboard() {
  delete[] keyboard;
  delete[] keyboardPC;
}

int Chip8Keyboard::keyBoardMap(char key) {
  for (size_t i{}; i < 16; i++) {
    if (keyboardPC[i] == key) {
      return i;
    }
  }
  return -1;
}

void Chip8Keyboard::chip8keydown(int key) { keyboard[key] = true; }
void Chip8Keyboard::chip8keyboardUp(int key) { keyboard[key] = false; }
bool Chip8Keyboard::keyIsDown(int key) const { return keyboard[key]; }
