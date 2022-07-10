#pragma once
#include "constants.h"
class Chip8Stack {

public:
  unsigned short stack[totalStackDepth];
  void chip8StackPush(unsigned short val, unsigned char &SP);
  unsigned short chip8StackPop(unsigned char &SP);
};
