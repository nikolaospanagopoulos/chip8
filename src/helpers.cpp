#include "../includes/helpers.h"
#include "../includes/CustomException.hpp"
#include <SDL2/SDL_timer.h>
#include <fstream>
#include <iostream>
#include <sstream>

void delay(unsigned char &delayTimer) {
  if (delayTimer > 0) {
    SDL_Delay(3);
    delayTimer--;
  }
}

char *readFile(const char *filename, size_t &filesize) {

  std::ifstream inFile{};

  inFile.open(filename, std::ios::in | std::ios::binary);

  if (!inFile) {
    throw CustomException((char *)"there was a problem opening the file");
  }

  inFile.seekg(0, inFile.end);
  long int length = inFile.tellg();
  inFile.seekg(0, inFile.beg);

  char *buffer = new char[length];

  inFile.read(buffer, length);

  inFile.close();

  filesize = length;
  std::cout << buffer << std::endl;
  return buffer;
}
