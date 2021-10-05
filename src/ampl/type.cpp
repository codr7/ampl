#include "ampl/sym.hpp"
#include "ampl/type.hpp"

namespace ampl {
  Type::Id Type::next_id = 0;

  Type::Type(const Sym &name, initializer_list<Type> parent_types): id(next_id++), name(name) {
    assert(id < MAX_COUNT);
    methods.is_true = [](auto &val) { return true; };
    this->parent_types[id] = true;

    for (Type pt: parent_types) {
      for (Id i = 0; i <= pt.id; i++) { this->parent_types[i] |= pt.parent_types[i]; }
    }
  }

  bool operator==(const Type &lhs, const Type &rhs) { return lhs.id == rhs.id; }
  bool operator!=(const Type &lhs, const Type &rhs) { return lhs.id != rhs.id; } 

  ostream &operator <<(ostream &out, const Type &type) {
    out << type.name;
    return out;
  }
}
