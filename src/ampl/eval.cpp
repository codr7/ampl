#include "ampl/vm.hpp"

/*
  #define AMPL_TRACE(name)						\
  cout << #name << endl;						\
*/

#define AMPL_TRACE(name)

#define AMPL_DISPATCH(prev)						\
  goto *dispatch[(op = &ops[pc++])->code];

namespace ampl {

  bool VM::eval(PC start_pc) {
    static const void* dispatch[] = {&&GOTO, &&PUSH, &&STOP};

    PC pc = start_pc;
    Op *op = nullptr;
    AMPL_DISPATCH(pc);

  GOTO: {
      AMPL_TRACE(GOTO);
      AMPL_DISPATCH(op->as<ops::Goto>().pc);
    }

  PUSH: {
      AMPL_TRACE(PUSH);
      push(op->as<ops::Push>().val);
      AMPL_DISPATCH(pc);
    }

  STOP: {
      AMPL_TRACE(STOP);
    }
  
    return true;
  }

}
