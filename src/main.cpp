#include <stdio.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "timer.h"
#include "texture.h"

int main() {
  if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
    printf("Could not initialize SDL! Error: %s\n", SDL_GetError());
  }
  if(IMG_Init(IMG_INIT_PNG) < 0) {
    printf("Could not initialize IMG! Error: %s\n", IMG_GetError());
  }

  int window_width = 640;
  int window_height = 480;

  SDL_Window* window = SDL_CreateWindow("Space Impact clone", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_SHOWN);
  SDL_assert(window);

  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); //| SDL_RENDERER_PRESENTVSYNC);
  SDL_assert(renderer);

  struct Texture player;
  player.load_from_file(renderer, "assets/gfx/ship_white.png");
  player.dst = {window_width/15 - player.width/2, window_height/2 - player.height/2, player.width, player.height};
  player.xvel = 0; player.yvel = 0;
  player.speed = 3;
  
  struct Texture enemy;
  enemy.load_from_file(renderer, "assets/gfx/space_enemy.png");
  enemy.dst = {window_width - enemy.width/2, window_height/2 - enemy.height/2, enemy.width / 2, enemy.height};
  enemy.xvel = -1; enemy.yvel = 0;
  enemy.speed = 2;

  SDL_Event event;
  bool is_running = true;
  int fps = 60;
  int ticks_per_frame = 1000 / fps;
  struct Timer fps_timer;
  struct Timer cap_timer;
  int counted_frames = 0;
  fps_timer.start();
  while(is_running) {
    cap_timer.start();
    if(SDL_PollEvent(&event) && event.type == SDL_QUIT) {
      is_running = false;
    }
    // Events here
    switch(event.type) {
    case SDL_KEYDOWN:
      switch(event.key.keysym.sym) {
      case SDLK_LEFT:
        player.xvel = -1;
        break;
      case SDLK_RIGHT:
        player.xvel = 1;
        break;
      case SDLK_UP:
        player.yvel = -1;
        break;
      case SDLK_DOWN:
        player.yvel = 1;
        break;
      }
      break;
    case SDL_KEYUP:
      switch(event.key.keysym.sym) {
      case SDLK_LEFT:
        if(player.xvel < 0)
          player.xvel = 0;
        break;
      case SDLK_RIGHT:
        if(player.xvel > 0)
          player.xvel = 0;
        break;
      case SDLK_UP:
        if(player.yvel < 0)
          player.yvel = 0;
        break;
      case SDLK_DOWN:
        if(player.yvel > 0)
          player.yvel = 0;
        break;
      }
      break;
    default:
      break;
    }

    float avg_fps = counted_frames / (fps_timer.get_ticks() / 1000.0f);
    if(avg_fps > 2000000) {
      avg_fps = 0;
    }

    player.dst.x += player.xvel * player.speed;
    player.dst.y += player.yvel * player.speed;

    // player boundaries
    if(player.dst.x + player.dst.w > window_width) {
      player.dst.x = window_width - player.dst.w;
    }
    if(player.dst.x < 0) {
      player.dst.x = 0;
    }
    if(player.dst.y + player.dst.h > window_height) {
      player.dst.y = window_height - player.dst.h;
    }
    if(player.dst.y < 0) {
      player.dst.y = 0;
    }

    enemy.dst.x += enemy.xvel * enemy.speed;

    // Paint the bg black
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    // render here
    player.render(renderer, player.dst.x, player.dst.y, nullptr);

    int sprite = (SDL_GetTicks() / 100) % 2;
    SDL_Rect e_anim = {sprite * enemy.width / 2, 0, enemy.width / 2, enemy.height};
    enemy.render(renderer, enemy.dst.x, enemy.dst.y, &e_anim);

    SDL_RenderPresent(renderer);
    ++counted_frames;
    int frameticks = cap_timer.get_ticks();
    if(frameticks < ticks_per_frame) {
      SDL_Delay(ticks_per_frame - frameticks);
    }
  }
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);

  IMG_Quit();
  SDL_Quit();

  return 0;
}
