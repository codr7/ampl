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
  
  struct Func {
    using Id = uint64_t;

    struct Arg {
      Arg(const optional<Sym> &name, const Type &type);
      optional<Sym> name;
      Type type;
    };
    
    static Id next_id;
    
    using Body = function<PC (const Func &self, const Pos &pos, PC ret_pc, VM &vm)>;

    Func(const Sym &name, const vector<Arg> &args, const vector<Type> &rets, const Body &body = nullptr);
    void emit(const Form &body_form, deque<Form> &in, VM &vm);
    bool is_applicable(VM &vm) const;
    PC eval(const Pos &pos, PC ret_pc, VM &vm) const;
    
    Id id;
    Sym name;
    vector<Arg> args;
    vector<Type> rets;
    int min_reg = 0, max_reg = 0;
    Body body;
  };

  bool operator==(const Func &lhs, const Func &rhs);
  ostream &operator <<(ostream &out, const Func &func);
}

#endif
