#include <ostream>
#include "ampl/pos.hpp"

namespace ampl {
  const Pos Pos::INVALID;

  ostream &operator <<(ostream &out, const Pos &pos) {
    out << "in '" << pos.source << "' at line " << pos.line << ", column " << pos.column;
    return out;
  }
}
