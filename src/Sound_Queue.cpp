// Nes_Snd_Emu 0.1.7. http://www.slack.net/~ant/

/* Copyright (C) 2005 by Shay Green. Permission is hereby granted, free of
charge, to any person obtaining a copy of this software module and associated
documentation files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use, copy, modify,
merge, publish, distribute, sublicense, and/or sell copies of the Software, and
to permit persons to whom the Software is furnished to do so, subject to the
following conditions: The above copyright notice and this permission notice
shall be included in all copies or substantial portions of the Software. THE
SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */

#include "Sound_Queue.h"

#include <cstring>

Sound_Queue::Sound_Queue() { free_sem = SDL_CreateSemaphore(buf_count - 1); }

Sound_Queue::~Sound_Queue()
{
  if (free_sem) SDL_DestroySemaphore(free_sem);
}

inline Sound_Queue::sample_t* Sound_Queue::buf(int index)
{
  return bufs.get() + index * buf_size;
}

void Sound_Queue::write(const sample_t* in, int count)
{
  while (count) {
    int n = buf_size - write_pos;
    if (n > count) n = count;

    memcpy(buf(write_buf) + write_pos, in, n * sizeof(sample_t));
    in += n;
    write_pos += n;
    count -= n;

    if (static_cast<size_t>(write_pos) >= buf_size) {
      write_pos = 0;
      write_buf = (write_buf + 1) % buf_count;
      SDL_SemWait(free_sem);
    }
  }
}

void Sound_Queue::fill_buffer(std::uint8_t* out, int count)
{
  if (SDL_SemValue(free_sem) < buf_count - 1) {
    memcpy(out, buf(read_buf), count);
    read_buf = (read_buf + 1) % buf_count;
    SDL_SemPost(free_sem);
  } else {
    memset(out, 0, count);
  }
}
