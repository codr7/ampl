#include <iostream>

#include "ampl/frame.hpp"
#include "ampl/vm.hpp"

namespace ampl {
  Frame::Frame(const Func &target, PC ret_pc, VM &vm): target(target), ret_pc(ret_pc) {
    Env &env = vm.env();
    
    move(env.stack.begin() + env.stack.size() - target.args.size(),
	 env.stack.end(),
	 back_inserter(stack));
    
    copy(env.regs.begin(), env.regs.begin() + target.min_reg, regs.begin());
  }

  PC Frame::ret(const Pos &pos, VM &vm) {
    if (!target.rets.empty()) {
      if (stack.size() < target.rets.size()) {
	throw EvalError(pos, "Not enough return values: ", stack);
      }
      
      Val *sv = &stack.back();
      
      for (Type *rv = &target.rets.back(); rv >= &target.rets.front(); rv--, sv--) {
	assert(sv >= &stack.front());
	if (!sv->type.isa(*rv)) { throw EvalError(pos, "Wrong return type: ", *sv); }
      }
            
      move(stack.begin() + stack.size() - target.rets.size(),
	   stack.end(),
	   back_inserter(vm.env().stack));
    }
    
    return ret_pc;
  }
}
