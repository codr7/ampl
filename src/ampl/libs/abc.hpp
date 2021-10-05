#ifndef AMPL_LIBS_ABC
#define AMPL_LIBS_ABC

#include "ampl/stack.hpp"
#include "ampl/type.hpp"

namespace ampl {
  struct VM;
}

namespace ampl::libs {
  using namespace ampl;

  struct ABC {
    using BoolType = TType<bool>;
    using IntType = TType<int>;
    using StackType = TType<Stack>;
    
    ABC(VM &vm);

    BoolType bool_type;
    IntType int_type;
    StackType stack_type;
  };
}

#endif
