#ifndef AMPL_OP_HPP
#define AMPL_OP_HPP

#include "ampl/form.hpp"
#include "ampl/types.hpp"

namespace ampl {
  using namespace std;

  struct VM;
  
  struct Op {
    Op(Form &form);
    virtual PC eval(VM &vm) = 0;

    Form &form;
  };
}

#endif
