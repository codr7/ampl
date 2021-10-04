#include "ampl/sym.hpp"

namespace ampl {
  Sym::Id Sym::next_id = 0;

  Sym::Sym(const string_view &name): id(next_id++), name(name) {}
}
