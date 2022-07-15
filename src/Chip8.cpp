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

Chip8::Chip8() : memory(), keyboard(), registers(), screen() {

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

  unsigned short tmp = 0;
  unsigned char y = (opcode >> 4) & 0x000f;
  unsigned char finalFourBits = opcode & 0x000f;

  switch (finalFourBits) {
  case 0x00:
    std::cout << "case lal1";
    registers.V[x] = registers.V[y];
    break;
  case 0x01:
    std::cout << "case lal2";
    registers.V[x] = registers.V[x] | registers.V[y];
    break;
  case 0x02:
    std::cout << "case lal3";
    registers.V[x] = registers.V[x] & registers.V[y];
    break;
  case 0x03:
    std::cout << "case lal4";
    registers.V[x] = registers.V[x] ^ registers.V[y];
    break;
  case 0x04:
    std::cout << "case lal5";
    tmp = registers.V[x] + registers.V[y];
    registers.V[0x0f] = false;
    if (tmp > 0xff) {
      registers.V[0x0f] = true;
    }

    registers.V[x] = tmp;
    break;

  // 8xy5 - SUB Vx, Vy. Set vx = Vx - Vy, set VF = Not borrow
  case 0x05:
    std::cout << "case lal5";
    registers.V[0x0f] = false;
    if (registers.V[x] > registers.V[y]) {
      registers.V[0x0f] = true;
    }
    registers.V[x] = registers.V[x] - registers.V[y];
    break;

  // 8xy6 - SHR Vx {, Vy}
  case 0x06:
    std::cout << "case lal5";
    registers.V[0x0f] = registers.V[x] & 0x01;
    registers.V[x] = registers.V[x] / 2;
    break;

  // 8xy7 - SUBN Vx, Vy
  case 0x07:
    std::cout << "case lal5";
    registers.V[0x0f] = registers.V[y] > registers.V[x];
    registers.V[x] = registers.V[y] - registers.V[x];
    break;

  // 8xyE - SHL Vx {, Vy}
  case 0x0E:
    registers.V[0x0f] = registers.V[x] & 0b10000000;
    std::cout << "case lal5";
    registers.V[x] = registers.V[x] * 2;
    break;
  }
}
void Chip8::execExtended(unsigned short &opcode) {
  unsigned char x = (opcode >> 8) & 0x000f;

  unsigned char kk = opcode & 0x00ff;

  unsigned char y = (opcode >> 4) & 0x000f;
  unsigned char n = opcode & 0x000f;
  unsigned short nnn = opcode & 0x0fff;
  // see only first 4 bits
  switch (opcode & 0xf000) {

  case 0x1000:

    std::cout << "case 3";
    registers.PC = nnn;
    break;
    // add to stack counter , pc to nn
  case 0x2000:
    std::cout << "case 4";
    stack.chip8StackPush(registers.PC, registers.SP);
    registers.PC = nnn;
    break;
    //  skip next instruction if Vx=kk
  case 0x3000:
    std::cout << "case 5";
    if (registers.V[x] == kk) {
      registers.PC += 2;
    }
    break;
  case 0x4000:
    std::cout << "case 6";
    if (registers.V[x] != kk) {
      registers.PC += 2;
    }
    break;
    // skip next instruction if Vx = Vy
  case 0x5000:
    std::cout << "case 7";
    if (registers.V[x] == registers.V[y]) {
      registers.PC += 2;
    }
    break;
    // Vx == kk
  case 0x6000:
    std::cout << "case 8";
    registers.V[x] = kk;
    break;
    // set Vx = Vx +kk
  case 0x7000:
    std::cout << "case 9";
    registers.V[x] += kk;
    break;
  case 0x8000:
    std::cout << "case 10<<";
    execExtended8(opcode);
    break;

  case 0x9000:
    std::cout << "case 20<<";
    if (registers.V[x] != registers.V[y]) {
      registers.PC += 2;
    }
    break;

  // Annn - LD I, addr. Sets the I register to nnn
  case 0xA000:
    std::cout << "case 30<<";
    registers.I = nnn;
    break;

  // bnnn - Jump to location nnn + V0
  case 0xB000:
    std::cout << "case 40<<";
    registers.PC = nnn + registers.V[0x00];
    break;

  // Cxkk - RND Vx, byte
  case 0xC000:
    std::cout << "case 50<<";
    srand(clock());
    registers.V[x] = (rand() % 255) & kk;
    break;

  case 0xD000: {
    std::cout << "case 60<<";
    const unsigned int *sprite = &memory.memory[registers.I];
    registers.V[0x0f] =
        screen.drawSprite(registers.V[x], registers.V[y], sprite, n);
  } break;
  }
}

void Chip8::chip8Exec(unsigned short opcode) {

  switch (opcode) {
  case 0x00e0:
    std::cout << "case 1" << std::endl;
    screen.chip8screenClear();
    break;
  case 0x00ee:
    std::cout << "case 2" << std::endl;
    registers.PC = stack.chip8StackPop(registers.SP);
    break;
  default:
    execExtended(opcode);
  }
}
