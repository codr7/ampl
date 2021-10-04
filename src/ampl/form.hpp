#ifndef AMPL_FORM_HPP
#define AMPL_FORM_HPP

#include "ampl/pos.hpp"

namespace ampl {
  using namespace std;
  
  struct Form {
    Form(Pos pos);
    virtual ~Form();
    virtual void dump(ostream &out) const = 0;

    Pos pos;
  };
}

#endif
