#ifndef __TIMER_H_
#define __TIMER_H_

/*  Got this code from https://lazyfoo.net/tutorials/SDL/23_advanced_timers/index.php */

#include <cstdint>
#include <SDL2/SDL.h>

struct Timer {
  uint32_t m_start_ticks, m_paused_ticks;
  bool m_paused, m_started;

  Timer() {
    m_start_ticks = 0;
    m_paused_ticks = 0;

    m_paused = false;
    m_started = false;
  }

  void start() {
    m_started = true;
    m_paused = false;
    m_start_ticks = SDL_GetTicks();
    m_paused_ticks = 0;
  }

  void stop() {
    m_started = false;
    m_paused = false;
    m_start_ticks = 0;
    m_paused_ticks = 0;
  }

  void pause() {
    if(m_started && !m_paused) {
      m_paused = true;
      m_paused_ticks = SDL_GetTicks() - m_start_ticks;
      m_start_ticks = 0;
    }
  }

  void unpause() {
    if(m_started && m_paused) {
      m_paused = false;
      m_start_ticks = SDL_GetTicks() - m_paused_ticks;
      m_paused_ticks = 0;
    }
  }

  uint32_t get_ticks() {
    uint32_t time = 0;
    if(m_started) {
      if(m_paused) {
        time = m_paused_ticks;
      } else {
        time = SDL_GetTicks() - m_start_ticks;
      }
    }
    return time;
  }
};

#endif // __TIMER_H_
