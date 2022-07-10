#include "Chip8.h"
#include "Chip8Keyboard.h"
#include "Chip8Screen.h"
#include "CustomException.h"
#include "constants.h"
#include <SDL2/SDL.h>
#include <iostream>
int main()

{

  try {

    Chip8 chip8;

    Chip8Keyboard *keyboard = &chip8.keyboard;

    Chip8Screen *screen = &chip8.screen;

    chip8.screen.chip8ScreenSet(0, 0);
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
    } while (!ended);
    SDL_DestroyWindow(window);

  } catch (CustomException &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  return 0;
}
