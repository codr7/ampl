#ifndef AMPL_RBUF_HPP
#define AMPL_RBUF_HPP

#include <array>

namespace ampl {
  using namespace std;
  
  template <typename T, size_t MAX>
  struct RBuf {
    size_t rpos = 0, wpos = 0, count = 0;
    array<T, MAX> items;

    void push(const T &it) {
      assert(count < MAX);
      count++;

      if (wpos == MAX) { wpos = 0; }
      items[wpos++] = it;
    }

    T pop() {
      assert(count);
      count--;

      if (rpos == MAX) { rpos = 0; }
      return items[rpos++];
    }
  };
}

#endif
