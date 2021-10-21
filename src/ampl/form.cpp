#include "ampl/form.hpp"

namespace ampl {
  Form pop_front(deque<Form> &in) {
    Form f = move(in.front());
    in.pop_front();
    return f;
  }

  ostream &operator <<(ostream &out, const Form &form) {
    form.dump(out);
    return out;
  }
}
