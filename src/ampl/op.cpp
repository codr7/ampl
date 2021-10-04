#include "ampl/op.hpp"

namespace ampl::ops {
  Op make_goto(const Form &form, PC pc) {
    return Op(form, AMPL_GOTO, Goto(pc));
  }

  const Op &stop() {
    static Op op(ampl::forms::stop(), AMPL_STOP, Void());
    return op;
  }
}

