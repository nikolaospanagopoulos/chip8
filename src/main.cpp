#include "../includes/Chip8.hpp"
#include "../includes/Chip8Memory.hpp"
#include "../includes/Chip8Registers.hpp"
#include "../includes/CustomException.hpp"
#include "../includes/constants.h"
#include "../includes/helpers.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <bitset>
#include <iostream>
#include <stdio.h>

int main(int argc, char **argv) {

  Chip8 *chip8 = new Chip8;
  Chip8Keyboard *keyboard = chip8->getKeyboard();
  Chip8Screen *screen = chip8->getScreen();
  Memory *memory = chip8->getMemory();
  Registers *registers = chip8->getRegisters();
  try {

    if (argc < 2) {
      throw CustomException((char *)"you must provide a file");
    }
    size_t filesize;

    char *buffer = readFile(argv[1], filesize);

    chip8->chip8load(buffer, filesize);

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow(

        "CHIP8 WINDOW", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        CHIP8_WIDTH * 10, CHIP8_HEIGHT * 10, SDL_WINDOW_SHOWN);

    SDL_Renderer *renderer =
        SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET);
    while (1) {

      SDL_Event event;
      while (SDL_PollEvent(&event)) {

        switch (event.type) {
        case SDL_QUIT:
          goto out;

          break;
        case SDL_KEYUP: {
          char key = event.key.keysym.sym;
          char vkey = keyboard->keyBoardMap(key);

          if (vkey != -1) {
            keyboard->chip8keyboardUp(vkey);
          }

        } break;
        case SDL_KEYDOWN: {
          char key = event.key.keysym.sym;
          char vkey = keyboard->keyBoardMap(key);

          if (vkey != -1) {
            keyboard->chip8keydown(vkey);
          }

        } break;
        }
      }

      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
      SDL_RenderClear(renderer);
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);

      for (int x = 0; x < CHIP8_WIDTH; x++) {

        for (int y = 0; y < CHIP8_HEIGHT; y++) {
          if (screen->screenIsSet(x, y)) {
            SDL_Rect r;
            r.x = x * 10;
            r.y = y * 10;
            r.w = 10;
            r.h = 10;
            SDL_RenderFillRect(renderer, &r);
          }
        }
      }

      SDL_RenderPresent(renderer);
      delay(registers->delayTimer);

      unsigned short opcode = memory->memoryGetShort(registers->PC);
      registers->PC += 2;
      chip8->chip8Exec(opcode);
      std::cout << opcode << std::endl;
    }

  out:

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    delete chip8;
    std::cout << "app finished\n";
    return 0;
  } catch (CustomException &e) {

    std::cerr << e.what() << std::endl;

    delete chip8;
    return 1;
  }
}
