#include "ampl/type.hpp"
#include "ampl/val.hpp"
#include "ampl/vm.hpp"

namespace ampl {
  void Val::dump(ostream &out) const {
    assert(type.methods.dump);
    type.methods.dump(*this, out);
  }

  bool Val::is_true() const {
    assert(type.methods.is_true);
    return type.methods.is_true(*this);
  }

  ostream &operator <<(ostream &out, const Val &val) {
    val.dump(out);
    return out;
  }
}
