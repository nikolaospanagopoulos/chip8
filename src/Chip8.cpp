#include "Chip8.h"
#include "CustomException.h"
#include "constants.h"
#include <bitset>

const int chip8DefaultCharSet[] = {

    0xf0, 0x90, 0x90, 0x90, 0xf0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xf0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F

};

Chip8::Chip8() {

  mempcpy(&memory.memory, chip8DefaultCharSet, sizeof(chip8DefaultCharSet));
}

void Chip8::chp8Load(const char *buf, size_t size) {
  // we dont start from beggining of memory so we need to calc offset
  if (size + programLoadAddress > chip8MemorySize)

  {
    throw CustomException(const_cast<char *>("loaded buffer overflows memory"));
  }

  memcpy(&memory.memory[programLoadAddress], buf, size);
  registers.PC = programLoadAddress;
}
void Chip8::execExtended8(unsigned short &opcode) {

  unsigned char x = (opcode >> 8) & 0x000f;

  unsigned char y = (opcode >> 4) & 0x000f;
  unsigned char finalFourBits = opcode & 0x000f;

  switch (finalFourBits) {
  case 0x00:
    registers.V[x] = registers.V[y];
    break;
  }
}
void Chip8::execExtended(unsigned short &opcode) {
  unsigned char x = (opcode >> 8) & 0x000f;

  unsigned char kk = opcode & 0x00ff;

  unsigned char y = (opcode >> 4) & 0x000f;

  unsigned short nnn = opcode & 0x0fff;
  // see only first 4 bits
  switch (opcode & 0xf000) {

  case 0x1000:

    registers.PC = nnn;
    break;
    // add to stack counter , pc to nn
  case 0x2000:
    stack.chip8StackPush(registers.PC, registers.SP);
    registers.PC = nnn;
    break;
    //  skip next instruction if Vx=kk
  case 0x3000:
    if (registers.V[x] == kk) {
      registers.PC += 2;
    }
    break;
  case 0x4000:
    if (registers.V[x] != kk) {
      registers.PC += 2;
    }
    break;
    // skip next instruction if Vx = Vy
  case 0x5000:
    if (registers.V[x] == registers.V[y]) {
      registers.PC += 2;
    }
    break;
    // Vx == kk
  case 0x6000:
    registers.V[x] = kk;
    break;
    // set Vx = Vx +kk
  case 0x7000:
    registers.V[x] += kk;
    break;
  case 0x8000:
    execExtended8(opcode);
    break;
  }
}

void Chip8::chip8Exec(unsigned short opcode) {

  switch (opcode) {
  case 0x00e0:
    screen.chip8screenClear();
    break;
  case 0x00ee:
    registers.PC = stack.chip8StackPop(registers.SP);
    break;
  default:
    execExtended(opcode);
  }
}
