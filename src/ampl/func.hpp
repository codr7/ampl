#ifndef AMPL_FUNC_HPP
#define AMPL_FUNC_HPP

#include <deque>
#include <optional>
#include <vector>

#include "ampl/ampl.hpp"
#include "ampl/sym.hpp"
#include "ampl/type.hpp"

namespace ampl {
  struct Form;
  struct Pos;
  struct VM;

  enum CallFlags { CALL_CHECK = 1, CALL_DROP = 4, CALL_TCO = 8};

  struct Func {
    using Id = uint64_t;

    struct Arg {
      Arg(const optional<Sym> &name, const Type &type);
      optional<Sym> name;
      Type type;
    };
    
    static Id next_id;
    
    using Body = function<PC (const Func &self, CallFlags flags, const Pos &pos, PC ret_pc, VM &vm)>;

    struct Imp {
      Imp(const Sym &name, const vector<Arg> &args, const vector<Type> &rets, const Body &body);

      Imp(const Func &func,
	  const Sym &name,
	  const vector<Arg> &args,
	  const vector<Type> &rets,
	  PC start_pc,
	  Reg min_reg);
      
      Id id;
      Sym name;
      vector<Arg> args;
      vector<Type> rets;
      PC start_pc;
      int min_reg;
      Body body;
    };
      
    Func(const Sym &name, const vector<Arg> &args, const vector<Type> &rets, const Body &body);

    Func(const Sym &name,
	 const vector<Arg> &args,
	 const vector<Type> &rets,
	 PC start_pc,
	 Reg min_reg);
    
    void emit(const Form &body_form, deque<Form> &in, VM &vm);
    bool is_applicable(VM &vm) const;

    PC eval(CallFlags flags, const Pos &pos, PC ret_pc, VM &vm) const {
      return (flags & CALL_TCO) ? imp->start_pc : imp->body(*this, flags, pos, ret_pc, vm);
    }

    shared_ptr<const Imp> imp;
  };

  bool operator==(const Func &lhs, const Func &rhs);
  ostream &operator <<(ostream &out, const Func &func);
}

#endif
