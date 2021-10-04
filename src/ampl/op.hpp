#ifndef AMPL_OP_HPP
#define AMPL_OP_HPP

#include "ampl/form.hpp"
#include "ampl/types.hpp"

namespace ampl {
  using namespace std;

  struct VM;

  struct Op {
    enum Code {AMPL_GOTO, AMPL_PUSH, AMPL_STOP};
    Op(Code code, const Form &form);
    Code code;
    Form form;
  };
}

#endif
