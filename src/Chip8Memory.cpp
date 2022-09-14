#include "../includes/Chip8Memory.hpp"
#include "../includes/CustomException.hpp"
#include <iostream>

Memory::Memory() { memory = new u_int8_t[CHIP8_MEMORY_SIZE]; }
Memory::~Memory() {
  std::cout << "destroyed memory \n";
  delete[] memory;
  memory = nullptr;
}

void Memory::setMemory(int index, unsigned char value) {

  checkIndex(index);
  memory[index] = value;
}

unsigned char Memory::getMemory(int index) const {

  checkIndex(index);
  return memory[index];
}

unsigned short Memory::memoryGetShort(int index) {
  unsigned char byte1 = getMemory(index);
  unsigned char byte2 = getMemory(index + 1);

  return byte1 << 8 | byte2;
}

unsigned char *Memory::getAllMemory() const { return memory; }

bool Memory::checkIndex(const int &index) const {

  if (index < 0 || index > CHIP8_MEMORY_SIZE) {

    throw CustomException((char *)"invalid index");
    return false;
  }
  return true;
}
