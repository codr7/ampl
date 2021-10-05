#include "ampl/error.hpp"
#include "ampl/vm.hpp"

/*
  #define TRACE(name)						\
  cout << #name << endl;						\
*/

#define TRACE(name)

#define DISPATCH(next_pc)				\
  goto *dispatch[(op = &ops[(pc = (next_pc))])->code];

namespace ampl {

  bool VM::eval(PC start_pc) {
    static const void* dispatch[] = {&&BRANCH, &&GOTO, &&PUSH, &&STOP};

    PC pc = start_pc;
    Op *op = nullptr;
    DISPATCH(pc);

  BRANCH: {
      TRACE(BRANCH);
      auto &branch = op->as<ops::Branch>();
      bool ok = false;

      {
	auto v = pop();
	if (!v) { throw EvalError(branch.form.pos, "Missing branch condition"); }
	ok = v->is_true();
      }
      
      DISPATCH(ok ? pc+1 : branch.false_pc);
    }

  GOTO: {
      TRACE(GOTO);
      DISPATCH(op->as<ops::Goto>().pc);
    }

  PUSH: {
      TRACE(PUSH);
      push(op->as<ops::Push>().val);
      DISPATCH(pc+1);
    }

  STOP: {
      TRACE(STOP);
    }
  
    return true;
  }

}
