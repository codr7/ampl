#ifndef AMPL_LIBS_ABC
#define AMPL_LIBS_ABC

#include "ampl/func.hpp"
#include "ampl/lib.hpp"
#include "ampl/macro.hpp"
#include "ampl/stack.hpp"
#include "ampl/type.hpp"

namespace ampl {
  struct VM;
}

namespace ampl::libs {
  using namespace ampl;

  struct ABC: Lib {
    using BoolType = TType<bool>;
    using FuncType = TType<Func>;
    using IntType = TType<int>;
    using MacroType = TType<Macro>;
    using StackType = TType<Stack>;
    
    ABC(VM &vm);

    Type any_type;
    
    BoolType bool_type;
    FuncType func_type;
    IntType int_type;
    MacroType macro_type;
    StackType stack_type;
  };
}

#endif
