#include <iostream>

#include "ampl/error.hpp"
#include "ampl/timer.hpp"
#include "ampl/vm.hpp"

#define DISPATCH(next_pc)				\
  goto *dispatch[(op = &ops[(pc = (next_pc))])->code];

namespace ampl {

  bool VM::eval(PC start_pc) {
    static const void* dispatch[] = {
      &&BENCH, &&BRANCH,
      &&CALL, &&COPY,
      &&DEC, &&DROP, &&EQUAL, &&GOTO, &&GTLIT, &&LOAD, &&LTLIT, &&NOP, &&PUSH, &&RET, &&STORE,
      //---STOP---
      &&STOP};

    PC pc = start_pc;
    Op *op = nullptr;
    DISPATCH(pc);

  BENCH: {
      auto &bench = op->as<ops::Bench>();

      {
	Timer t;
	for (int i = 0; i < bench.reps; i++) { eval(pc+1); }
	push(libs.abc.int_type, (int)t.ms());
      }
      
      DISPATCH(bench.end_pc);
    }
    
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

      if ((call.flags & CALL_CHECK) && !call.target.is_applicable(*this)) {
	throw EvalError(call.form.pos, "Not applicable: ", call.target, '\n', stack());
      }

      DISPATCH(call.target.eval(call.flags, call.form.pos, pc+1, *this));
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

  DEC: {
      peek().as<int>() -= op->as<ops::Dec>().delta;
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

  GTLIT: {
      {
	auto &gtlit = op->as<ops::GtLit>();
	optional<Val> y(gtlit.y ? gtlit.y : pop()), x(gtlit.x ? gtlit.x : pop());
	if (!x || !y) { throw EvalError(gtlit.form.pos, "Missing arg"); }
	push(libs.abc.bool_type, x->compare(*y) == GT);
      }
      
      DISPATCH(pc+1);
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

  LTLIT: {
      {
	auto &ltlit = op->as<ops::LtLit>();
	optional<Val> y(ltlit.y ? ltlit.y : pop()), x(ltlit.x ? ltlit.x : pop());
	if (!x || !y) { throw EvalError(ltlit.form.pos, "Missing arg"); }
	push(libs.abc.bool_type, x->compare(*y) == LT);
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
	env().regs[store.reg] = (store.flags & ops::Store::PEEK) ? peek(store.offset) : pop(store.offset);
      }
      
      DISPATCH(pc+1);
    }

    //---STOP---

  STOP: {}
    
    return true;
  }

}
