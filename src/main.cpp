#include "Chip8.h"
#include "Chip8Keyboard.h"
#include "CustomException.h"
#include "constants.h"
#include <SDL2/SDL.h>
#include <iostream>
int main()

{

  try {

    Chip8 chip8;

    Chip8Keyboard keyboard = chip8.keyboard;

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
        } else if (event.type == SDL_KEYDOWN) {
          char key = event.key.keysym.sym;
          int vkey = keyboard.mapKeyboard(key);
          if (vkey != -1) {
            keyboard.key_down(vkey);
            std::cout << vkey << std::endl;
          }
        } else if (event.type == SDL_KEYUP) {

          char key = event.key.keysym.sym;
          int vkey = keyboard.mapKeyboard(key);
          if (vkey != -1) {
            keyboard.key_up(vkey);
          }
        }
      }
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
      SDL_RenderClear(renderer);
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
      SDL_Rect r;
      r.x = 0;
      r.y = 0;
      r.w = 40;
      r.h = 40;
      SDL_RenderDrawRect(renderer, &r);
      SDL_RenderPresent(renderer);
    } while (!ended);
    SDL_DestroyWindow(window);
    return 0;

  } catch (CustomException &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}
