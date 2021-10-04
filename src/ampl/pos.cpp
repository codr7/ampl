#include "ampl/pos.hpp"

namespace ampl {
  const Pos Pos::INVALID;

  Pos::Pos(const string_view source, int row, int col): source(source), row(row), col(col) {}
}
