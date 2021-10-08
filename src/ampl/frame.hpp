#ifndef AMPL_FRAME_HPP
#define AMPL_FRAME_HPP

#include "ampl/func.hpp"

namespace ampl {
  struct Frame {  
    Frame(const Func &target, PC ret_pc, VM &vm);
    PC ret(const Pos &pos, VM &vm);
    
    Func target;
    PC ret_pc;
  };
}
  
#endif
