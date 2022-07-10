#pragma once
#include "constants.h"
#include <array>

class Chip8Memory {

public:
  std::array<unsigned char, chip8MemorySize> memory{};

  void chip8MemorySet(int &&index, unsigned char &&val);
  unsigned char chip8memoryGet(int &&index) const;
};
