#ifndef AMPL_OP_HPP
#define AMPL_OP_HPP

#include <variant>
#include "ampl/ampl.hpp"
#include "ampl/form.hpp"
#include "ampl/val.hpp"

namespace ampl {
  using namespace std;

  struct VM;
  enum OpCode {BRANCH = 0, GOTO, PUSH, STOP};

  namespace ops {
    struct Branch {
      static const OpCode CODE = GOTO;      

      Branch(const Form &form, PC false_pc): form(form), false_pc(false_pc) {}
      
      Form form;
      PC false_pc;
    };

    struct Goto {
      static const OpCode CODE = GOTO;      

      Goto(const Form &form, PC pc): form(form), pc(pc) {}
      
      Form form;
      PC pc;
    };
      
    struct Push {
      static const OpCode CODE = PUSH;      

      template <typename T>
      Push(const Form &form, const TType<T> &type, const T &data): form(form), val(type, data) {}

      Push(const Form &form, const Val &val): form(form), val(val) {}
      
      Form form;
      Val val;
    };

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
    variant<ops::Branch, ops::Goto, ops::Push, ops::Stop> data;
  };
}

#endif
