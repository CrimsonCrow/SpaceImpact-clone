#ifndef __TEXTURE_H_
#define __TEXTURE_H_

/*  Also got this from Lazyfoo.net */

#include <string>
#include <cstdint>
#include <SDL.h>
#include <SDL_image.h>

struct Texture {
  SDL_Texture* texture;
  SDL_Rect dst = {0,0,0,0};

  int width, height;
  int xvel, yvel;
  float speed;


  Texture() {
    texture = nullptr;

    width = 0; height = 0;
    xvel = 0; yvel = 0;
    speed = 0;
  }
  ~Texture() {
    free();
  }

  bool load_from_file(SDL_Renderer* renderer, std::string path){
    free();
    SDL_Texture* new_texture = nullptr;

    SDL_Surface* loaded_surface = IMG_Load(path.c_str());
    //SDL_assert(loaded_surface);

    //SDL_SetColorKey(loaded_surface, SDL_TRUE, SDL_MapRGB(loaded_surface->format, 0, 0xFF, 0xFF));

    new_texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);
    SDL_assert(new_texture);

    width = loaded_surface->w;
    height = loaded_surface->h;

    SDL_FreeSurface(loaded_surface);

    texture = new_texture;
    return texture != nullptr;
  }

  void free(){
    if(texture != nullptr) {
      SDL_DestroyTexture(texture);
      texture = nullptr;
      width = 0;
      height = 0;
    }
  }

  void set_color(uint8_t red, uint8_t green, uint8_t blue) {
    SDL_SetTextureColorMod(texture, red, green, blue);
  }

  void set_blend_mode(SDL_BlendMode blend) {
    SDL_SetTextureBlendMode(texture, blend);
  }

  void set_alpha(uint8_t alpha) {
    SDL_SetTextureAlphaMod(texture, alpha);
  }

  void render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip) {
    SDL_Rect render_quad = {x, y, width, height};
    if(clip != nullptr) {
      render_quad.w = clip->w;
      render_quad.h = clip->h;
    }
    SDL_RenderCopy(renderer, texture, clip, &render_quad);
  }
};

#endif
