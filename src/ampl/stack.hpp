#ifndef AMPL_STACK_HPP
#define AMPL_STACK_HPP

#include <vector>

namespace ampl {
  using namespace std;

  struct Val;
  
  using Stack = vector<Val>;

  void dump(const Stack &stack, ostream &out);
  ostream &operator <<(ostream &out, const Stack &stack);
}

#endif
