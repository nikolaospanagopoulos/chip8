#include "Chip8Stack.h"
#include "CustomException.h"
#include "constants.h"

static void stackInBounds(unsigned char &SP) {

  if (SP > totalStackDepth) {
    throw CustomException(const_cast<char *>("stack goes beyond limit"));
  }
}

void Chip8Stack::chip8StackPush(unsigned short val, unsigned char &SP) {

  stackInBounds(SP);
  stack[SP] = val;
  SP += 1;
}

unsigned short Chip8Stack::chip8StackPop(unsigned char &SP) {
  SP -= 1;
  stackInBounds(SP);
  unsigned short val = stack[SP];
  return val;
}
