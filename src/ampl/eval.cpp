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
    static const void* dispatch[] = {
      &&BRANCH, &&GOTO, &&LOAD, &&PUSH, &&STORE,
      //---STOP---
      &&STOP};

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

  LOAD: {
      TRACE(LOAD);

      {
	auto &load = op->as<ops::Load>();
	auto v = proc().regs[load.reg];
	if (!v) { throw EvalError(load.form.pos, "Nothing to load"); }
	push(*v);
      }
      
      DISPATCH(pc+1);
    }

  PUSH: {
      TRACE(PUSH);
      push(op->as<ops::Push>().val);
      DISPATCH(pc+1);
    }

  STORE: {
      TRACE(STORE);
      
      {
	auto &store = op->as<ops::Store>();
	auto v = pop();
	if (!v) { throw EvalError(store.form.pos, "Stack is empty"); }
	proc().regs[store.reg] = v;
      }
      
      DISPATCH(pc+1);
    }

    //---STOP---

  STOP: {
      TRACE(STOP);
    }
  
    return true;
  }

}
