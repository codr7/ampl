#include <cassert>

#include "ampl/rbuf.hpp"

using namespace ampl;

static void rbuf_tests() {
  RBuf<int, 3> buf;

  buf.push(1);
  buf.push(2);
  assert(buf.count == 2);
  assert(buf.pop() == 1);
  
  buf.push(3);
  buf.push(4);
  assert(buf.count == 3);
  assert(buf.pop() == 2);
  assert(buf.pop() == 3);
  assert(buf.pop() == 4);
  assert(buf.count == 0);
}

int main() {  
  rbuf_tests();
  return 0;
}
