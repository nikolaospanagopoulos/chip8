#include "../includes/Chip8Stack.hpp"
#include "../includes/CustomException.hpp"

Stack::Stack() { stack = new unsigned short[CHIP8_STACK_SIZE]; }

Stack::~Stack() { delete[] stack; }

unsigned short *Stack::getStack() const { return stack; }

unsigned short Stack::stackPop(unsigned char &SP) {

  SP--;
  checkSpBounds(SP);

  return stack[SP];
}

void Stack::chip8StackPush(unsigned short val, unsigned char &SP) {

  checkSpBounds(SP);

  stack[SP] = val;
  SP += 1;
  std::cout << SP << std::endl;
}

void Stack::checkSpBounds(const unsigned char &SP) const {

  if (SP >= CHIP8_STACK_SIZE) {
    throw CustomException((char *)"stack size overflow");
  }
}
