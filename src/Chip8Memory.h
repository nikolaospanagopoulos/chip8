#pragma once
#include "constants.h"
#include <array>

class Chip8Memory {

public:
  // std::array<unsigned int, chip8MemorySize> memory{};
  unsigned int memory[chip8MemorySize];

  void chip8MemorySet(int &&index, unsigned char &&val);
  unsigned char chip8memoryGet(int index) const;
  unsigned short chip8memoryGetShort(int index);
};
