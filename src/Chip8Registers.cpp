#include "../includes/Chip8Registers.hpp"
#include <iostream>

Registers::Registers() {

  I = 0;
  delayTimer = 0;
  soundTimer = 0;
  PC = 0;
  SP = 0;

  V = new unsigned char[CHIP8_REGISTERS];
}

Registers::~Registers() { delete[] V; }
