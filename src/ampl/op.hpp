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
    BENCH = 0, BRANCH, CALL, COPY, DROP, EQUAL, GOTO, LOAD, NOP, PUSH, RET, STORE,
    //---STOP---
    STOP};

  namespace ops {
    struct Bench {
      static const OpCode CODE = BENCH;      

      Bench(const Form &form, int reps, PC end_pc = -1): form(form), reps(reps), end_pc(end_pc) {}
      
      Form form;
      int reps;
      PC end_pc;
    };

    struct Branch {
      static const OpCode CODE = BRANCH;      

      Branch(const Form &form, PC false_pc = -1): form(form), false_pc(false_pc) {}
      
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

      Goto(const Form &form, PC pc = -1): form(form), pc(pc) {}
      
      Form form;
      PC pc;
    };

    struct Load {
      static const OpCode CODE = LOAD;      

      Load(const Form &form, Reg reg): form(form), reg(reg) {}
      
      Form form;
      Reg reg;
    };

    struct Nop {
      static const OpCode CODE = NOP;
    };

    struct Push {
      static const OpCode CODE = PUSH;      

      template <typename T>
      Push(const Form &form, const TType<T> &type, const T &data): form(form), val(type, data) {}

      Push(const Form &form, const Val &val): form(form), val(val) {}
      
      Form form;
      Val val;
    };

    struct Ret {
      static const OpCode CODE = RET;      

      Ret(const Form &form): form(form) {}
      
      Form form;
    };

    struct Store {
      static const OpCode CODE = STORE;      

      Store(const Form &form, Reg reg, size_t offset = 0): form(form), reg(reg), offset(offset) {}
      
      Form form;
      Reg reg;
      size_t offset;
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
    variant<ops::Bench, ops::Branch,
	    ops::Call, ops::Copy,
	    ops::Drop, ops::Equal, ops::Goto, ops::Load, ops::Nop, ops::Push, ops::Ret, ops::Store,
	    //---STOP---
	    ops::Stop> data;
  };
}

#endif
