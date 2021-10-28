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

  struct BoolType: TType<bool> {
    struct Imp: Type::Imp {
      Imp(const Sym &name, initializer_list<Type> parent_types);
    };
    
    BoolType(const Sym &name, initializer_list<Type> parent_types);
  };

  struct FormType: TType<Form> {
    struct Imp: Type::Imp {
      Imp(const Sym &name, initializer_list<Type> parent_types);
    };
    
    FormType(const Sym &name, initializer_list<Type> parent_types);
  };

  struct FuncType: TType<Func> {
    struct Imp: Type::Imp {
      Imp(const Sym &name, initializer_list<Type> parent_types);
    };
    
    FuncType(const Sym &name, initializer_list<Type> parent_types);
  };

  struct IntType: TType<int> {
    struct Imp: Type::Imp {
      Imp(const Sym &name, initializer_list<Type> parent_types);
    };
    
    IntType(const Sym &name, initializer_list<Type> parent_types);
  };

  struct StackType: TType<Stack> {
    struct Imp: Type::Imp {
      Imp(const Sym &name, initializer_list<Type> parent_types);
    };
    
    StackType(const Sym &name, initializer_list<Type> parent_types);
  };

  struct SymType: TType<Sym> {
    struct Imp: Type::Imp {
      Imp(const Sym &name, initializer_list<Type> parent_types);
    };
    
    SymType(const Sym &name, initializer_list<Type> parent_types);
  };

  struct ABC: Lib {
    using MacroType = TType<Macro>;
    using MetaType = TType<Type>;
    using RegType = TType<Reg>;
    
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
