#include "ampl/vm.hpp"

/*
  #define AMPL_TRACE(name)						\
  cout << #name << endl;						\
*/

#define AMPL_TRACE(name)

#define AMPL_DISPATCH(prev)						\
  goto *dispatch[ops[pc++].code];

namespace ampl {

  bool VM::eval(PC start_pc) {
    static const void* dispatch[] = {&&GOTO, &&PUSH, &&STOP};

    PC pc = start_pc;
    AMPL_DISPATCH(pc);

  GOTO: {
      AMPL_TRACE(GOTO);
      
      AMPL_DISPATCH(pc);
    }

  PUSH: {
      AMPL_TRACE(PUSH);
      
      AMPL_DISPATCH(pc);
    }

  STOP: {
      AMPL_TRACE(STOP);
    }
  
    return true;
  }

}
