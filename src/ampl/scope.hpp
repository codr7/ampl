#ifndef AMPL_SCOPE_HPP
#define AMPL_SCOPE_HPP

#include <map>
#include "ampl/val.hpp"

namespace ampl {
  using namespace std;

  struct Sym;
  
  struct Scope {
    Scope(optional<Scope> parent_scope = nullopt);
    optional<Val> find(const Sym &key) const;
    
    uint64_t reg_count = 0;
    map<Sym, Val> bindings;
  };
}

#endif
