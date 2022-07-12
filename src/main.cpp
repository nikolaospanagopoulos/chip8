#include "Chip8.h"
#include "Chip8Keyboard.h"
#include "Chip8Screen.h"
#include "CustomException.h"
#include "constants.h"
#include <SDL2/SDL.h>
#include <filesystem>
#include <fstream>
#include <iostream>
int main(int argc, char **argv)

{

  try {

    Chip8 chip8;

    Chip8Keyboard *keyboard = &chip8.keyboard;

    Chip8Register *chip8register = &chip8.registers;
    Chip8Memory *memory = &chip8.memory;
    Chip8Screen *screen = &chip8.screen;

    if (argc < 2) {
      throw CustomException(const_cast<char *>("You must provide a filename"));
    }

    FILE *file = fopen(argv[1], "r");

    if (!file) {
      throw CustomException(const_cast<char *>("file doesnt exist"));
      return 1;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char buff[size];
    std::cout << buff << std::endl;
    int res = fread(buff, size, 1, file);

    if (res != 1) {
      throw CustomException(const_cast<char *>("cannot read from file"));
    }
    std::cout << res;
    std::cout << buff << std::endl;
    fclose(file);

    chip8.chp8Load(buff, size);

    std::cout << "pc" << chip8register->PC << std::endl;

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow(

        "chip8 window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640,
        320, SDL_WINDOW_SHOWN

    );

    SDL_Renderer *renderer =
        SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET);

    bool ended = false;
    do {
      SDL_Event event;
      while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
          ended = true;
          break;
        } else if (event.type == SDL_KEYDOWN) {
          char key = event.key.keysym.sym;
          int vkey = keyboard->mapKeyboard(key);
          if (vkey != -1) {
            keyboard->key_down(vkey);
            std::cout << vkey << std::endl;
          }
        } else if (event.type == SDL_KEYUP) {
          char key = event.key.keysym.sym;
          int vkey = keyboard->mapKeyboard(key);
          if (vkey != -1) {
            keyboard->key_up(vkey);
          }
        }
      }
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
      SDL_RenderClear(renderer);
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
      for (int x{}; x < chip8Width; x++) {
        for (int y{}; y < chip8Height; y++) {

          if (screen->chip8ScreenIsSet(x, y)) {
            SDL_Rect r;
            r.x = x * chip8Multiplier;
            r.y = y * chip8Multiplier;
            r.w = chip8Multiplier;
            r.h = chip8Multiplier;

            SDL_RenderFillRect(renderer, &r);
          }
        }
      }
      SDL_RenderPresent(renderer);
      if (chip8register->delayTimer > 0) {
        SDL_Delay(100);
        chip8register->delayTimer -= 1;
      }
      if (chip8register->soundTimer > 0) {
        chip8register->soundTimer -= 1;
      }
      unsigned short opcode = memory->chip8memoryGetShort(chip8register->PC);
      chip8register->PC += 2;
      chip8.chip8Exec(opcode);
      std::cout << opcode << std::endl;
    } while (!ended);
    SDL_DestroyWindow(window);

  } catch (CustomException &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  return 0;
}
