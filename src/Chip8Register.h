#pragma once

#include <array>
class Chip8Register {

public:
  // 16 8bit data registers
  std::array<unsigned char, 16> V{};
  // I register to store memory addresses
  unsigned short I;
  // sound timer
  unsigned char soundTimer;
  // delay timer
  unsigned char delayTimer;
  // program counter
  unsigned short PC;
  // stack pointer
  unsigned char SP;
};
