#ifndef AMPL_ENV_HPP
#define AMPL_ENV_HPP

#include <optional>
#include "ampl/ampl.hpp"
#include "ampl/val.hpp"
#include "ampl/stack.hpp"

namespace ampl {
  struct Env {  
    static const Reg REG_COUNT = 64;

    array<optional<Val>, REG_COUNT> regs;
    Stack stack;
  };
}
  
#endif
