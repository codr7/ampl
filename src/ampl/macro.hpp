#ifndef AMPL_MACRO_HPP
#define AMPL_MACRO_HPP

#include <deque>
#include "ampl/form.hpp"
#include "ampl/sym.hpp"

namespace ampl {
  struct Form;
  struct Pos;
  struct VM;
  
  struct Macro {
    using Id = uint64_t;
    static Id next_id;

    using Body = function<void (Macro &self, const Form &form, deque<Form> &in, VM &vm)>;

    Macro(const Sym &name, uint8_t arg_count, const Body &body);
    void expand(const Form &form, deque<Form> &in, VM &vm);
    
    Id id;
    Sym name;
    uint8_t arg_count;
    Body body;
  };

  bool operator==(const Macro &lhs, const Macro &rhs);
  ostream &operator <<(ostream &out, const Macro &macro);
}

#endif
