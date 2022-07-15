#pragma once
#include "Chip8Keyboard.h"
#include "Chip8Memory.h"
#include "Chip8Register.h"
#include "Chip8Screen.h"
#include "Chip8Stack.h"
class Chip8 {
public:
  Chip8Memory memory;

  Chip8Keyboard keyboard;
  Chip8Stack stack;
  Chip8Register registers;
  Chip8Screen screen;
  Chip8();
  void chp8Load(const char *buf, size_t size);
  void chip8Exec(unsigned short opcode);
  void execExtended(unsigned short &opcode);
  void execExtended8(unsigned short &opcode);
};
