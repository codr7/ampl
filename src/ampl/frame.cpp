#include <iostream>

#include "ampl/frame.hpp"
#include "ampl/vm.hpp"

namespace ampl {
  Frame::Frame(const Func &target, PC ret_pc, VM &vm): target(target), ret_pc(ret_pc) {
    vm.envs.reserve(vm.envs.size()+1);
    Env &src = vm.env(), &dst = vm.push_env();

    auto i = src.stack.begin() + src.stack.size() - target.imp->args.size(), j = src.stack.end();
    move(i, j, back_inserter(dst.stack));
    src.stack.erase(i, j);
    copy(src.regs.begin(), src.regs.begin() + target.imp->min_reg, dst.regs.begin());
  }

  PC Frame::ret(const Pos &pos, VM &vm) {
    unique_ptr<Env> src(vm.pop_env());
    
    if (!target.imp->rets.empty()) {
      if (src->stack.size() < target.imp->rets.size()) {
	throw EvalError(pos, "Not enough return values: ", src->stack);
      }
      
      Val *sv = &src->stack.back();
      
      for (const Type *rv = &target.imp->rets.back(); rv >= &target.imp->rets.front(); rv--, sv--) {
	assert(sv >= &src->stack.front());
	if (!sv->type.isa(*rv)) { throw EvalError(pos, "Wrong return type: ", *sv); }
      }

      auto i = src->stack.begin() + src->stack.size() - target.imp->rets.size(), j = src->stack.end();
      move(i, j, back_inserter(vm.env().stack));
    }
    
    return ret_pc;
  }
}
