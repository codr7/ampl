#ifndef AMPL_ERROR_HPP
#define AMPL_ERROR_HPP

#include <stdexcept>
#include "ampl/pos.hpp"

namespace ampl {
  using namespace std;
  
  struct Error: runtime_error {
    template <typename...Args>
    Error(const string &message, Args &&...args): runtime_error(message) {}
  };

  struct PosError: Error {
    template <typename...Args>
    PosError(const Pos &pos, const string &message, Args &&...args):
      Error(message, forward<Args>(args)...) {}
  };

  struct EmitError: PosError {
    template <typename...Args>
    EmitError(const Pos &pos, const string &message, Args &&...args):
      PosError(pos, message, forward<Args>(args)...) {}
  };

  struct EvalError: PosError {
    template <typename...Args>
    EvalError(const Pos &pos, const string &message, Args &&...args):
      PosError(pos, message, forward<Args>(args)...) {}
  };
}

#endif
