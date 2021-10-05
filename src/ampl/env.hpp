#ifndef AMPL_ENV_HPP
#define AMPL_ENV_HPP

#include <optional>
#include "ampl/ampl.hpp"

namespace ampl {
  const Reg REG_COUNT = 64;
  
  struct Env {  
    array<optional<Val>, REG_COUNT> regs;
    Stack stack;
  };
}
  
#endif
