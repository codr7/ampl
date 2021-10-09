#include "ampl/form.hpp"

namespace ampl {
  ostream &operator <<(ostream &out, const Form &form) {
    form.dump(out);
    return out;
  }
}
