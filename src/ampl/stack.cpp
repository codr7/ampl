#include <ostream>
#include "ampl/stack.hpp"
#include "ampl/val.hpp"

namespace ampl {
  void dump(const Stack &stack, ostream &out) {
    out << '[';

    for (auto i = stack.begin(); i != stack.end(); i++) {
      if (i != stack.begin()) { out << ' '; }
      i->dump(out);
    }
    
    out << ']';
  }
  
  ostream &operator <<(ostream &out, const Stack &stack) {
    dump(stack, out);
    return out;
  }
}
