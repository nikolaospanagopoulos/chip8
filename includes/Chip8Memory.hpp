#pragma once
#include "./constants.h"
#include <cstdint>

class Memory {

  uint8_t *memory;
  bool checkIndex(const int &index) const;

public:
  void setMemory(int index, unsigned char value);
  uint8_t getMemory(int index) const;
  uint8_t *getAllMemory() const;
  unsigned short memoryGetShort(int index);

  Memory();
  ~Memory();
};
