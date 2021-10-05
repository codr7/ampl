#include "ampl/error.hpp"

namespace ampl {
  Error::Error(const Pos &pos, const string &message): runtime_error(message) {}

  EmitError::EmitError(const Pos &pos, const string &message): Error(pos, message) {}

  EvalError::EvalError(const Pos &pos, const string &message): Error(pos, message) {}
}
