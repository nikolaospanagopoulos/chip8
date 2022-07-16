#pragma once
#include "constants.h"
#include <array>
#include <cstdint>

class Chip8Memory {

public:
  std::array<uint8_t, chip8MemorySize> memory{};

  void chip8MemorySet(int index, unsigned char &val);
  unsigned char chip8memoryGet(int index) const;
  unsigned short chip8memoryGetShort(int index);
};
