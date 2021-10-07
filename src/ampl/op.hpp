#ifndef AMPL_OP_HPP
#define AMPL_OP_HPP

#include <variant>
#include "ampl/ampl.hpp"
#include "ampl/form.hpp"
#include "ampl/func.hpp"
#include "ampl/val.hpp"

namespace ampl {
  using namespace std;

  struct VM;
  enum OpCode {
    BRANCH = 0, CALL, COPY, DROP, EQUAL, GOTO, LOAD, PUSH, STORE,
    //---STOP---
    STOP};

  namespace ops {
    struct Branch {
      static const OpCode CODE = BRANCH;      

      Branch(const Form &form, PC false_pc): form(form), false_pc(false_pc) {}
      
      Form form;
      PC false_pc;
    };

    struct Call {
      static const OpCode CODE = CALL;      

      Call(const Form &form, const Func &target): form(form), target(target) {}
      
      Form form;
      Func target;
    };

    struct Copy {
      static const OpCode CODE = COPY;      

      Copy(const Form &form): form(form) {}
      
      Form form;
    };

    struct Drop {
      static const OpCode CODE = DROP;      

      Drop(const Form &form, uint64_t count): form(form), count(count) {}
      
      Form form;
      uint64_t count;
    };
    
    struct Equal {
      static const OpCode CODE = EQUAL;      

      Equal(const Form &form, optional<Val> x = nullopt, const optional<Val> y = nullopt): form(form), x(x), y(y) {}
      
      Form form;
      optional<Val> x, y;
    };

    struct Goto {
      static const OpCode CODE = GOTO;      

      Goto(const Form &form, PC pc): form(form), pc(pc) {}
      
      Form form;
      PC pc;
    };

    struct Load {
      static const OpCode CODE = LOAD;      

      Load(const Form &form, Reg reg): form(form), reg(reg) {}
      
      Form form;
      Reg reg;
    };

    struct Push {
      static const OpCode CODE = PUSH;      

      template <typename T>
      Push(const Form &form, const TType<T> &type, const T &data): form(form), val(type, data) {}

      Push(const Form &form, const Val &val): form(form), val(val) {}
      
      Form form;
      Val val;
    };

    struct Store {
      static const OpCode CODE = STORE;      

      Store(const Form &form, Reg reg): form(form), reg(reg) {}
      
      Form form;
      Reg reg;
    };

    //---STOP---
    
    struct Stop {
      static const OpCode CODE = STOP;
    };
  }
  
  struct Op {
    template <typename T>
    Op(const T &data): code(T::CODE), data(data) { }

    template <typename T>
    T &as() { return get<T>(data); }

    template <typename T>
    const T &as() const { return get<T>(data); }

    OpCode code;
    variant<ops::Branch, ops::Call, ops::Copy, ops::Drop, ops::Equal, ops::Goto, ops::Load, ops::Push, ops::Store,
	    //---STOP---
	    ops::Stop> data;
  };
}

#endif
