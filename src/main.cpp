#include <stdio.h>
#include <array>
#include <random>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

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

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis_ewd(window_width+46, window_width + 1000);
  std::uniform_int_distribution<> dis_ehg(23, window_height-23);

  std::array<struct Texture, 15> enemy_container;
  for(auto& e : enemy_container) {
    e.load_from_file(renderer, "assets/gfx/space_enemy.png");
    e.dst = {dis_ewd(gen), dis_ehg(gen), e.width / 2, e.height};
    e.xvel = -1; e.yvel = 0;
    e.speed = 4;
  }

  std::array<struct Texture, 100> p_bullets;
  for(auto& b : p_bullets) {
    b.load_from_file(renderer, "assets/gfx/bullet.png");
    b.xvel = 1; b.yvel = 0;
    b.speed = 7;
  }

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

    // Paint the bg black
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    // render here
    player.render(renderer, player.dst.x, player.dst.y, nullptr);

    int sprite = (SDL_GetTicks() / 100) % 2;
    SDL_Rect e_anim = {sprite * 46, 0, 46, 23};
    for(auto& e : enemy_container) {
      e.dst.x += e.xvel * e.speed;
      if(e.dst.x < -e.width/2 || SDL_HasIntersection(&player.dst, &e.dst)) {
        e.dst = {dis_ewd(gen), dis_ehg(gen), e.width / 2, e.height};
      }
      e.render(renderer, e.dst.x, e.dst.y, &e_anim);
    }

    /*
    for(auto& b : p_bullets) {
      b.dst.x += b.xvel * b.speed;
      //if(event.key.keysym.sym == SDLK_SPACE) {
        b.render(renderer, player.dst.x + 39, player.dst.y + 13, nullptr);
      //}
    }
    */


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
