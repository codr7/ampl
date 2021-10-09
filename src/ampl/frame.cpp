#include <iostream>

#include "ampl/frame.hpp"
#include "ampl/vm.hpp"

namespace ampl {
  Frame::Frame(const Func &target, PC ret_pc, VM &vm): target(target), ret_pc(ret_pc) {
    vm.envs.reserve(vm.envs.size()+1);
    Env &src = vm.env(), &dst = vm.push_env();

    auto i = src.stack.begin() + src.stack.size() - target.args.size(), j = src.stack.end();
    move(i, j, back_inserter(dst.stack));
    src.stack.erase(i, j);
    copy(src.regs.begin(), src.regs.begin() + target.min_reg, dst.regs.begin());
  }

  PC Frame::ret(const Pos &pos, VM &vm) {
    Env src(vm.pop_env());
    
    if (!target.rets.empty()) {
      if (src.stack.size() < target.rets.size()) {
	throw EvalError(pos, "Not enough return values: ", src.stack);
      }
      
      Val *sv = &src.stack.back();
      
      for (Type *rv = &target.rets.back(); rv >= &target.rets.front(); rv--, sv--) {
	assert(sv >= &src.stack.front());
	if (!sv->type.isa(*rv)) { throw EvalError(pos, "Wrong return type: ", *sv); }
      }

      auto i = src.stack.begin() + src.stack.size() - target.rets.size(), j = src.stack.end();
      move(i, j, back_inserter(vm.env().stack));
      src.stack.erase(i, j);
    }
    
    return ret_pc;
  }
}
