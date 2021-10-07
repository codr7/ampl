#ifndef AMPL_FUNC_HPP
#define AMPL_FUNC_HPP

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
      optional<Sym> name;
      Type type;
    };
    
    static Id next_id;
    
    using Body = function<PC (Func &self, const Pos &pos, PC ret, VM &vm)>;

    Func(const Sym &name, const vector<Arg> &args, const vector<Type> &rets, const Body &body);
    PC call(const Pos &pos, PC ret, VM &vm);

    
    Id id;
    Sym name;
    vector<Arg> args;
    vector<Type> rets;
    Body body;
  };

  bool operator==(const Func &lhs, const Func &rhs);
  ostream &operator <<(ostream &out, const Func &func);
}

#endif
