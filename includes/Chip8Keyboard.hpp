#pragma once
#include <array>

class Chip8Keyboard {

  bool *keyboard;

public:
  const char *keyboardPC;
  Chip8Keyboard();
  ~Chip8Keyboard();
  int keyBoardMap(char key);
  bool *getKeyboard() const;
  void chip8keydown(int key);
  void chip8keyboardUp(int key);
  bool keyIsDown(int key) const;
};
