#ifndef AMPL_ERROR_HPP
#define AMPL_ERROR_HPP

#include <stdexcept>
#include "ampl/pos.hpp"

namespace ampl {
  using namespace std;
  
  struct Error: runtime_error {
    Error(const Pos &pos, const string &message);
  };

  struct EmitError: Error {
    EmitError(const Pos &pos, const string &message);
  };

  struct EvalError: Error {
    EvalError(const Pos &pos, const string &message);
  };
}

#endif
