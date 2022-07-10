#include "Chip8Memory.h"
#include "CustomException.h"

static void memoryInBounds(int index) {

  if (index < 0 || index > chip8MemorySize) {
    throw CustomException(const_cast<char *>("outside of bounds"));
  }
}

void Chip8Memory::chip8MemorySet(int &&index, unsigned char &&val) {
  memoryInBounds(index);
  memory[index] = val;
}

unsigned char Chip8Memory::chip8memoryGet(int &&index) const {
  memoryInBounds(index);
  return memory[index];
}
