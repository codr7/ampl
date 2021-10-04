#ifndef AMPL_SYM_HPP
#define AMPL_SYM_HPP

#include <string>

namespace ampl {
  using namespace std;
  
  struct Sym {
    using Id = uint64_t;
    static Id next_id;

    Sym(const string_view &name);

    Id id;
    string name;
  };

  bool operator==(const Sym &lhs, const Sym &rhs);
}

#endif
