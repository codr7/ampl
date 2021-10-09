#include "ampl/type.hpp"
#include "ampl/val.hpp"
#include "ampl/vm.hpp"

namespace ampl {
  Order Val::compare(const Val &other) const {
    if (type != other.type) { return ampl::compare(type.name, other.type.name); }
    assert(type.methods.compare);
    return type.methods.compare(*this, other);
  }
  
  void Val::dump(ostream &out) const {
    if (!type.methods.dump) { throw Error("Dump not implemented: ", type.name); }
    type.methods.dump(*this, out);
  }

  bool Val::is_true() const {
    assert(type.methods.is_true);
    return type.methods.is_true(*this);
  }

  bool operator==(const Val &lhs, const Val &rhs) {
    if (lhs.type != rhs.type) { return false; }
    auto is_equal = lhs.type.methods.is_equal;
    assert(is_equal);
    return is_equal(lhs, rhs);
  }

  bool operator!=(const Val &lhs, const Val &rhs) {
    return !(lhs == rhs);
  }
  
  ostream &operator <<(ostream &out, const Val &val) {
    val.dump(out);
    return out;
  }
}
