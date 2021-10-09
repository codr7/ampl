#include <iostream>

#include "ampl/error.hpp"
#include "ampl/vm.hpp"

#define DISPATCH(next_pc)				\
  goto *dispatch[(op = &ops[(pc = (next_pc))])->code];

namespace ampl {

  bool VM::eval(PC start_pc) {
    static const void* dispatch[] = {
      &&BRANCH,
      &&CALL, &&COPY,
      &&DROP, &&EQUAL, &&GOTO, &&LOAD, &&NOP, &&PUSH, &&RET, &&STORE,
      //---STOP---
      &&STOP};

    PC pc = start_pc;
    Op *op = nullptr;
    DISPATCH(pc);

  BRANCH: {
      auto &branch = op->as<ops::Branch>();
      bool ok = false;

      {
	auto v = try_pop();
	if (!v) { throw EvalError(branch.form.pos, "Missing branch condition"); }
	ok = v->is_true();
      }
      
      DISPATCH(ok ? pc+1 : branch.false_pc);
    }

  CALL: {
      auto &call = op->as<ops::Call>();

      if (!call.target.is_applicable(*this)) {
	throw EvalError(call.form.pos, "Not applicable: ", call.target, '\n', stack());
      }

      DISPATCH(call.target.eval(call.form.pos, pc+1, *this));
    }

  COPY: {
      {
	auto &copy = op->as<ops::Copy>();
	auto &s = stack();
	
	if (s.empty()) { throw EvalError(copy.form.pos, "Stack is empty"); }
	s.push_back(s.back());
      }

      DISPATCH(pc+1);
    }

  DROP: {
      drop(op->as<ops::Drop>().count);
      DISPATCH(pc+1);
    }

  EQUAL: {
      {
	auto &equal = op->as<ops::Equal>();
	optional<Val> y(equal.y ? equal.y : pop()), x(equal.x ? equal.x : pop());
	if (!x || !y) { throw EvalError(equal.form.pos, "Missing arg"); }
	push(libs.abc.bool_type, x == y);
      }
      
      DISPATCH(pc+1);
    }

  GOTO: {
      DISPATCH(op->as<ops::Goto>().pc);
    }

  LOAD: {
      {
	auto &load = op->as<ops::Load>();
	auto v = env().regs[load.reg];
	if (!v) { throw EvalError(load.form.pos, "Nothing to load"); }
	push(*v);
      }
      
      DISPATCH(pc+1);
    }

  NOP: {
      DISPATCH(pc+1);
    }

  PUSH: {
      push(op->as<ops::Push>().val);
      DISPATCH(pc+1);
    }

  RET: {
      DISPATCH(pop_frame().ret(op->as<ops::Ret>().form.pos, *this));
    }

  STORE: {
      {
	auto &store = op->as<ops::Store>();
	env().regs[store.reg] = pop(store.offset);
      }
      
      DISPATCH(pc+1);
    }

    //---STOP---

  STOP: {}
    
    return true;
  }

}
