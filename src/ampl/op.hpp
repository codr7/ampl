#ifndef AMPL_OP_HPP
#define AMPL_OP_HPP

#include <variant>
#include "ampl/form.hpp"
#include "ampl/types.hpp"
#include "ampl/val.hpp"

namespace ampl {
  using namespace std;

  struct VM;

  namespace ops {
    struct Goto {
      Goto(PC pc): pc(pc) {}
      
      PC pc;
    };
      
    struct Push {
      template <typename T>
      Push(const TType<T> &type, const T &data): val(type, data) {}
      
      Val val;
    };

    struct Void {
    };
  }
  
  enum Code {AMPL_GOTO = 0, AMPL_PUSH, AMPL_STOP};

  struct Op {
    static const int DATA_SIZE = 64;

    template <typename T>
    Op(const Form &form, Code code, const T &data): form(form), code(code), data(data) { }

    template <typename T>
    T &as() { return get<T>(data); }

    template <typename T>
    const T &as() const { return get<T>(data); }

    Form form;
    Code code;
    variant<ops::Goto, ops::Push, ops::Void> data;
  };

  namespace ops {
    Op make_goto(const Form &form, PC pc);

    template <typename T>
    Op make_push(const Form &form, const TType<T> &type, const T &data) {
      return Op(form, AMPL_PUSH, Push(type, data));
    }
  }
}

namespace ampl::ops {
  const Op &stop();
}

#endif
