#ifndef AMPL_LIBS_MATH_HPP
#define AMPL_LIBS_MATH_HPP

#include "ampl/lib.hpp"

namespace ampl {
  struct VM;
}

namespace ampl::libs {
  using namespace ampl;

  struct Math: Lib {
    Math(VM &vm);
    
    Func int_sub_func;
  };
}

#endif
