#include "../includes/Chip8.hpp"
#include <SDL2/SDL_events.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>

Chip8::Chip8() {
  memory = new Memory{};

  registers = new Registers{};

  stack = new Stack{};

  keyboard = new Chip8Keyboard{};

  screen = new Chip8Screen{};

  chip8DefaultCharSet = new unsigned char[80]{
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
  memcpy(memory->getAllMemory(), chip8DefaultCharSet, 160);
}
Chip8::~Chip8() {
  delete memory;
  delete stack;
  delete registers;
  delete screen;
  delete keyboard;
  delete chip8DefaultCharSet;
}
Chip8Screen *Chip8::getScreen() const { return screen; };
Chip8Keyboard *Chip8::getKeyboard() const { return keyboard; }
Memory *Chip8::getMemory() const { return memory; }
Registers *Chip8::getRegisters() const { return registers; }
Stack *Chip8::getStack() const { return stack; }

void Chip8::chip8load(const char *buffer, std::size_t size) {
  if (size + CHIP8_LOAD_ADDRESS > CHIP8_MEMORY_SIZE) {
    delete[] buffer;
    throw CustomException((char *)"there was a problem loading the file");
  }
  memcpy(&memory->getAllMemory()[CHIP8_LOAD_ADDRESS], buffer, size);
  getRegisters()->PC = CHIP8_LOAD_ADDRESS;

  delete[] buffer;
}

void Chip8::chip8ExecExtended(const unsigned short &opcode) {

  // we want last 12 bits
  unsigned short nnn = opcode & 0x0fff;
  uint8_t x = (opcode >> 8) & 0x000f;
  uint8_t kk = opcode & 0x00ff;
  uint8_t y = (opcode >> 4) & 0x000f;
  uint8_t n = opcode & 0x000f;

  switch (opcode & 0xf000) {
  // we only want the first four bits
  case 0x1000:
    registers->PC = nnn;
    break;

    // caqll subroutine at nnn
  case 0x2000:
    stack->chip8StackPush(registers->PC, registers->SP);
    registers->PC = nnn;

    break;
    // if Vx=kk -> skip next instruction
  case 0x3000:

    if (registers->V[x] == kk) {
      registers->PC += 2;
    }
    break;
    // skip next if Vx!=kk
  case 0x4000:
    if (registers->V[x] != kk) {
      registers->PC += 2;
    }
    break;
  case 0x5000:
    if (registers->V[x] == registers->V[y]) {
      registers->PC += 2;
    }
    break;

    // Vx=kk
  case 0x6000:
    registers->V[x] = kk;
    break;

  case 0x7000:
    registers->V[x] += kk;
    break;

  case 0x8000:
    chip8ExecExtendedEight(opcode);
    break;
  case 0x9000:
    if (registers->V[x] != registers->V[y]) {
      registers->PC += 2;
    }
    break;
  case 0xA000:
    registers->I = nnn;
    break;
  case 0xB000:
    registers->PC = nnn + registers->V[0x00];
    break;
  case 0xC000:
    srand(clock());
    registers->V[x] = (rand() % 255) & kk;
    break;
  case 0xD000: {
    const uint8_t *sprite = &memory->getAllMemory()[registers->I];
    registers->V[0x0f] =
        screen->drawSprite(registers->V[x], registers->V[y], sprite, n);

    break;
  }
  case 0xE000:

  {
    switch (opcode & 0x00ff) {
    case 0x9e:
      if (keyboard->keyIsDown(registers->V[x])) {
        registers->PC += 2;
      }
      break;
    case 0xa1:
      if (!keyboard->keyIsDown(registers->V[x])) {
        registers->PC += 2;
      }
      break;
    }

  }

  break;
  case 0xF000:
    chip8ExtendedF(opcode);
    break;
  }
}

void Chip8::chip8ExtendedF(const unsigned short &opcode) {

  uint8_t x = (opcode >> 8) & 0x000f;
  switch (opcode & 0x00ff) {
  case 0x07:

    registers->V[x] = registers->delayTimer;
    break;
  case 0x0A: {
    char pressedKey = waitForKeyPress();
    registers->V[x] = pressedKey;
  } break;
  case 0x15:
    registers->delayTimer = registers->V[x];
    break;
  case 0x18:
    registers->soundTimer = registers->V[x];
    break;
  case 0x1e:
    registers->I += registers->V[x];
    break;
  case 0x29:
    registers->I = registers->V[x] * 5;
    break;
  case 0x33: {
    uint8_t hundreds = registers->V[x] / 100;
    uint8_t tens = registers->V[x] / 10 % 10;
    uint8_t units = registers->V[x] % 10;
    memory->setMemory(registers->I, hundreds);
    memory->setMemory(registers->I + 1, tens);
    memory->setMemory(registers->I + 2, units);
  } break;
  case 0x55: {
    for (size_t i{}; i <= x; i++) {
      memory->setMemory(registers->I + i, registers->V[i]);
    }
  } break;
  case 0x65: {
    for (size_t i{}; i <= x; i++) {
      registers->V[i] = memory->getMemory(registers->I + i);
    }
  } break;
  }
}

char Chip8::waitForKeyPress() {
  SDL_Event event;
  while (SDL_WaitEvent(&event)) {
    if (event.type != SDL_KEYDOWN) {
      continue;
    }
    char c = event.key.keysym.sym;
    char chip8Key = keyboard->keyBoardMap(c);
    if (chip8Key != -1) {
      return chip8Key;
    }
  }
  return -1;
}

void Chip8::chip8ExecExtendedEight(const unsigned short &opcode) {

  uint8_t x = (opcode >> 8) & 0x000f;
  uint8_t y = (opcode >> 4) & 0x000f;
  uint8_t lastFourBits = opcode & 0x000f;
  uint8_t temp = 0;
  switch (lastFourBits) {
  // Vx = Vy
  case 0x00:
    registers->V[x] = registers->V[y];
    break;
  case 0x01:
    registers->V[x] = registers->V[x] | registers->V[y];
    break;
  case 0x02:
    registers->V[x] = registers->V[x] & registers->V[y];
    break;
  case 0x03:
    registers->V[x] = registers->V[x] ^ registers->V[y];
    break;
  case 0x04:
    temp = registers->V[x] + registers->V[y];
    registers->V[0x0f] = false;
    if (temp > 0xff) {
      registers->V[0x0f] = true;
    }
    registers->V[x] = temp;
    break;
  case 0x05:
    registers->V[0x0f] = false;
    if (registers->V[x] > registers->V[y]) {
      registers->V[0x0f] = true;
    }
    registers->V[x] = registers->V[x] - registers->V[y];
    break;
  case 0x06:
    registers->V[0x0f] = registers->V[x] & 0x01;
    registers->V[x] /= 2;
    break;
  case 0x07:
    if (registers->V[y] > registers->V[x]) {
      registers->V[0x0f] = 1;
    } else {
      registers->V[0x0f] = 0;
    }
    registers->V[x] = registers->V[x] - registers->V[y];
    break;
  case 0x0E:
    registers->V[0x0f] = registers->V[x] & 0b10000000;
    registers->V[x] *= 2;
    break;
  }
}

void Chip8::chip8Exec(const unsigned short &opcode) {

  Chip8Screen *screen = getScreen();
  Stack *stack = getStack();
  Registers *registers = getRegisters();
  switch (opcode) {
  // clear screen
  case 0x00E0:
    screen->screenClear();
    break;
    // return from sr
  case 0x00EE:
    registers->PC = stack->stackPop(registers->SP);
    break;
  default:
    chip8ExecExtended(opcode);
  }
}
