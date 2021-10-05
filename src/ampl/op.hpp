#ifndef AMPL_OP_HPP
#define AMPL_OP_HPP

#include <variant>
#include "ampl/ampl.hpp"
#include "ampl/form.hpp"
#include "ampl/val.hpp"

namespace ampl {
  using namespace std;

  struct VM;
  enum OpCode {GOTO = 0, PUSH, STOP};

  namespace ops {
    struct Goto {
      static const OpCode CODE = GOTO;      

      Goto(Form form, PC pc): form(form), pc(pc) {}
      
      Form form;
      PC pc;
    };
      
    struct Push {
      static const OpCode CODE = PUSH;      

      template <typename T>
      Push(Form form, const TType<T> &type, const T &data): form(form), val(type, data) {}
      
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
    variant<ops::Goto, ops::Push, ops::Stop> data;
  };
}

#endif
