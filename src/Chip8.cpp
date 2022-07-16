#include "Chip8.h"
#include "CustomException.h"
#include "SDL2/SDL.h"
#include "constants.h"
#include <bitset>
#include <stdint.h>

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
    registers.V[x] = registers.V[y];
    break;
  case 0x01:
    registers.V[x] = registers.V[x] | registers.V[y];
    break;
  case 0x02:
    registers.V[x] = registers.V[x] & registers.V[y];
    break;
  case 0x03:
    registers.V[x] = registers.V[x] ^ registers.V[y];
    break;
  case 0x04:
    tmp = registers.V[x] + registers.V[y];
    registers.V[0x0f] = false;
    if (tmp > 0xff) {
      registers.V[0x0f] = true;
    }

    registers.V[x] = tmp;
    break;

  // 8xy5 - SUB Vx, Vy. Set vx = Vx - Vy, set VF = Not borrow
  case 0x05:
    registers.V[0x0f] = false;
    if (registers.V[x] > registers.V[y]) {
      registers.V[0x0f] = true;
    }
    registers.V[x] = registers.V[x] - registers.V[y];
    break;

  // 8xy6 - SHR Vx {, Vy}
  case 0x06:
    registers.V[0x0f] = registers.V[x] & 0x01;
    registers.V[x] = registers.V[x] / 2;
    break;

  // 8xy7 - SUBN Vx, Vy
  case 0x07:
    registers.V[0x0f] = registers.V[y] > registers.V[x];
    registers.V[x] = registers.V[y] - registers.V[x];
    break;

  // 8xyE - SHL Vx {, Vy}
  case 0x0E:
    registers.V[0x0f] = registers.V[x] & 0b10000000;
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

  case 0x9000:
    if (registers.V[x] != registers.V[y]) {
      registers.PC += 2;
    }
    break;

  // Annn - LD I, addr. Sets the I register to nnn
  case 0xA000:
    registers.I = nnn;
    break;

  // bnnn - Jump to location nnn + V0
  case 0xB000:
    registers.PC = nnn + registers.V[0x00];
    break;

  // Cxkk - RND Vx, byte
  case 0xC000:
    srand(clock());
    registers.V[x] = (rand() % 255) & kk;
    break;

  case 0xD000: {
    const char *sprite = (const char *)&memory.memory[registers.I];

    registers.V[0x0f] =
        screen.drawSprite(registers.V[x], registers.V[y], sprite, n);
  } break;
  case 0xE000: {

    switch (opcode & 0x00ff) {
    case 0x9e:
      // skip next instruction if key with Vx is pressed
      if (keyboard.key_is_down(registers.V[x])) {
        registers.PC += 2;
      }
      break;
    case 0xa1:
      if (!keyboard.key_is_down(registers.V[x])) {

        registers.PC += 2;
      }
      break;
    }
    break;
  }
  case 0xF000:
    extendedF(opcode);
    break;
  }
}

void Chip8::chip8Exec(unsigned short &opcode) {

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

void Chip8::extendedF(unsigned short &opcode) {
  unsigned char x = (opcode >> 8) & 0x000f;
  switch (opcode & 0x00ff) {
  case 0x07:
    registers.V[x] = registers.delayTimer;
    break;
  case 0x0A: {

    char pressedKey = keyboard.waitForKeyPress();
    registers.V[x] = pressedKey;

  } break;
  case 0x15:
    registers.delayTimer = registers.V[x];
    break;
  case 0x18:
    registers.soundTimer = registers.V[x];
    break;
  case 0x1e:
    registers.I += registers.V[x];

    break;
  case 0x29:
    registers.I = registers.V[x] * 5;
    break;
  case 0x33: {
    uint8_t hundreds = registers.V[x] / 100;
    uint8_t tens = registers.V[x] / 10 % 10;
    /*
        uint8_t units = registers.V[x] % 10;
        memory.chip8MemorySet((int)registers.I, hundreds);
        memory.chip8MemorySet((int)registers.I + 1, tens);
        memory.chip8MemorySet((int)registers.I + 2, units);
            */
  } break;
  case 0x55:
    for (size_t i{}; i <= x; i++) {
      memory.chip8MemorySet(registers.I + i, registers.V[i]);
    }
    break;
  case 0x65: {

    for (size_t i{}; i <= x; i++) {
      registers.V[i] = memory.chip8memoryGet(registers.I + i);
    }

  } break;
  }
}
