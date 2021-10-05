#ifndef AMPL_PROC_HPP
#define AMPL_PROC_HPP

#include <optional>
#include "ampl/ampl.hpp"

namespace ampl {
  const Reg REG_COUNT = 64;
  
  struct Proc {  
    array<optional<Val>, REG_COUNT> regs;
    Stack stack;
  };
}
  
#endif
