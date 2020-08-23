#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <string>
#include <stdio.h>
#include "timer.h"

struct Player {
  SDL_Texture* texture;
  SDL_Rect src;
  SDL_Rect dst;

  int x, y, xvel, yvel;
  float speed;
};

SDL_Texture* load_texture(SDL_Renderer* renderer, std::string path);

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

  struct Player player;
  player.texture = nullptr;
  player.src = {0, 0, 46,31};
  player.dst = {window_width/15 - 46/2, window_height/2 - 31/2, 46,31};
  player.xvel = 0; player.yvel = 0;
  player.speed = 3;

  SDL_Event event;
  bool is_running = true;
  int fps = 60;
  int ticks_per_frame = 1000 / fps;
  struct LTimer fps_timer;
  struct LTimer cap_timer;
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

    // Paint the bg black
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    // render here
    player.texture = load_texture(renderer, "assets/gfx/ship_white.png");
    SDL_RenderCopy(renderer, player.texture, &player.src, &player.dst);

    SDL_RenderPresent(renderer);
    ++counted_frames;
    int frameticks = cap_timer.get_ticks();
    if(frameticks < ticks_per_frame) {
      SDL_Delay(ticks_per_frame - frameticks);
    }
  }

  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();

  return 0;
}

SDL_Texture* load_texture(SDL_Renderer* renderer, std::string path){
  SDL_Texture* new_texture = nullptr;
  SDL_Surface* loaded_surface = IMG_Load(path.c_str());
  SDL_assert(loaded_surface);

  new_texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);
  SDL_assert(new_texture);
  SDL_FreeSurface(loaded_surface);

  return new_texture;
}
