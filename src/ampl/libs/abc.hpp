#ifndef AMPL_LIBS_ABC_HPP
#define AMPL_LIBS_ABC_HPP

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
    using FormType = TType<Form>;
    using FuncType = TType<Func>;
    using IntType = TType<int>;
    using MacroType = TType<Macro>;
    using MetaType = TType<Type>;
    using RegType = TType<Reg>;
    using StackType = TType<Stack>;
    using SymType = TType<Sym>;
    
    ABC(VM &vm);

    Type any_type;
    
    BoolType bool_type;
    FormType form_type;
    FuncType func_type;
    IntType int_type;
    MacroType macro_type;
    MetaType meta_type;
    RegType reg_type;
    StackType stack_type;
    SymType sym_type;
  };
}

#endif
