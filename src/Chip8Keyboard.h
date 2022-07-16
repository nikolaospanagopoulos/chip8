#pragma once
#include "SDL2/SDL.h"
#include <array>
class Chip8Keyboard {

public:
  std::array<bool, 16> keyboard{};

  std::array<char, 16> keyboardMap{
      SDLK_0, SDLK_1, SDLK_2, SDLK_3, SDLK_4, SDLK_5, SDLK_6, SDLK_7,
      SDLK_8, SDLK_9, SDLK_a, SDLK_b, SDLK_c, SDLK_d, SDLK_e, SDLK_f};
  int mapKeyboard(char &key);
  void key_down(int key);
  void key_up(int key);
  bool key_is_down(int key);
  char waitForKeyPress();
};
