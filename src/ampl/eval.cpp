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
      &&BRANCH, &&COPY, &&DROP, &&EQUAL, &&GOTO, &&LOAD, &&PUSH, &&STORE,
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
	auto v = try_pop();
	if (!v) { throw EvalError(branch.form.pos, "Missing branch condition"); }
	ok = v->is_true();
      }
      
      DISPATCH(ok ? pc+1 : branch.false_pc);
    }

  COPY: {
      TRACE(COPY);

      {
	auto &copy = op->as<ops::Copy>();
	auto &s = stack();
	
	if (s.empty()) { throw EvalError(copy.form.pos, "Stack is empty"); }
	s.push_back(s.back());
      }

      DISPATCH(pc+1);
    }

  DROP: {
      TRACE(DROP);
      drop(op->as<ops::Drop>().count);
      DISPATCH(pc+1);
    }

  EQUAL: {
      TRACE(EQUAL);

      {
	auto &equal = op->as<ops::Equal>();
	optional<Val> y(equal.y ? equal.y : pop()), x(equal.x ? equal.x : pop());
	if (!x || !y) { throw EvalError(equal.form.pos, "Missing arg"); }
	push(libs.abc.bool_type, x == y);
      }
      
      DISPATCH(pc+1);
    }

  GOTO: {
      TRACE(GOTO);
      DISPATCH(op->as<ops::Goto>().pc);
    }

  LOAD: {
      TRACE(LOAD);

      {
	auto &load = op->as<ops::Load>();
	auto v = env().regs[load.reg];
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
	auto v = try_pop();
	if (!v) { throw EvalError(store.form.pos, "Stack is empty"); }
	env().regs[store.reg] = v;
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
