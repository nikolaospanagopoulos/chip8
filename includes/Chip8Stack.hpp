#pragma once
#include "constants.h"

class Stack {

  unsigned short *stack;

  void checkSpBounds(const unsigned char &SP) const;

public:
  unsigned short *getStack() const;
  unsigned short stackPop(unsigned char &SP);
  void chip8StackPush(unsigned short val, unsigned char &SP);
  Stack();
  ~Stack();
};
