#pragma once
#include "./constants.h"

class Registers {

public:
  unsigned char *V;
  unsigned short I;
  unsigned char delayTimer;
  unsigned char soundTimer;
  unsigned short PC;
  unsigned char SP;

  Registers();
  ~Registers();
};
