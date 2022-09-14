#pragma once
#include "Chip8Keyboard.hpp"
#include "Chip8Memory.hpp"
#include "Chip8Registers.hpp"
#include "Chip8Screen.hpp"
#include "Chip8Stack.hpp"
#include "CustomException.hpp"
#include "constants.h"
#include <cstdint>
class Chip8 {

  Chip8Screen *screen;
  Memory *memory;
  Registers *registers;
  Stack *stack;
  Chip8Keyboard *keyboard;

public:
  const uint8_t *chip8DefaultCharSet;
  void chip8load(const char *buffer, std::size_t size);
  void chip8Exec(const unsigned short &opcode);
  void chip8ExecExtended(const unsigned short &opcode);
  Chip8Keyboard *getKeyboard() const;
  Memory *getMemory() const;
  Registers *getRegisters() const;
  Chip8Screen *getScreen() const;
  void chip8ExecExtendedEight(const unsigned short &opcode);
  void chip8ExtendedF(const unsigned short &opcode);
  Stack *getStack() const;
  char waitForKeyPress();
  Chip8();
  ~Chip8();
};
