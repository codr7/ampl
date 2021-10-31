#ifndef AMPL_ERROR_HPP
#define AMPL_ERROR_HPP

#include <sstream>
#include <stdexcept>
#include "ampl/pos.hpp"

namespace ampl {
  using namespace std;

  template <typename Arg, typename...Args>
  string wstr(Arg &&arg, Args &&...args) {
    stringstream buf;
    buf << arg;
    ((buf << std::forward<Args>(args)), ...);
    return buf.str();
  }

  template <typename Arg, typename...Args>
  string str(Arg &&arg, Args &&...args) {
    stringstream buf;
    buf << arg;
    ((buf << std::forward<Args>(args)), ...);
    return buf.str();
  }

  struct Error: runtime_error {
    template <typename...Args>
    Error(const string &message, Args &&...args): runtime_error(wstr(message, forward<Args>(args)...)) {}
  };

  struct PosError: Error {
    template <typename...Args>
    PosError(const Pos &pos, const string &message, Args &&...args):
      Error(str("Error ", pos, ": ", wstr(message, forward<Args>(args)...))) {}
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

  struct ReadError: PosError {
    template <typename...Args>
    ReadError(const Pos &pos, const string &message, Args &&...args):
      PosError(pos, message, forward<Args>(args)...) {}
  };
}

#endif
