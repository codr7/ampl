#include <iostream>

#include "ampl/frame.hpp"
#include "ampl/vm.hpp"

namespace ampl {
  Frame::Frame(const Func &target, CallFlags flags, PC ret_pc, VM &vm): target(target), flags(flags), ret_pc(ret_pc) {
    vm.envs.reserve(vm.envs.size()+1);
    Env &src = vm.env(), &dst = vm.push_env();

    if (!target.imp->args.empty()) {
      auto i = src.stack.begin() + src.stack.size() - target.imp->args.size(), j = src.stack.end();
      move(i, j, back_inserter(dst.stack));
      src.stack.erase(i, j);
    }

    copy(src.regs.begin(), src.regs.begin() + target.imp->min_reg, dst.regs.begin());
  }

  PC Frame::ret(const Pos &pos, VM &vm) {
    unique_ptr<Env> src(vm.pop_env());
    
    if (!target.imp->rets.empty()) {
      if (src->stack.size() < target.imp->rets.size()) { throw EvalError(pos, "Not enough ret values: ", src->stack); }
      Val *sv = &src->stack.back();
      
      for (const Type *rv = &target.imp->rets.back(); rv >= &target.imp->rets.front(); rv--, sv--) {
	if (sv < &src->stack.front()) { throw EvalError(pos, "Missing ret value"); }
	if (!sv->type.isa(*rv)) { throw EvalError(pos, "Wrong ret type: ", *sv); }
      }

      if (!(flags & CALL_DROP)) {
	auto i = src->stack.begin() + src->stack.size() - target.imp->rets.size(), j = src->stack.end();
	move(i, j, back_inserter(vm.env().stack));
      }
    }
    
    return ret_pc;
  }
}
