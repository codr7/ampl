#ifndef AMPL_FORM_HPP
#define AMPL_FORM_HPP

#include <any>
#include <ostream>

#include "ampl/pos.hpp"

namespace ampl {
  using namespace std;

  struct VM;
  
  struct Form {
    struct Type {
      virtual void dump(const Form &form, ostream &out) const = 0;
      virtual void emit(const Form &form, VM &vm) const = 0;
    };

    template <typename T>
    struct TType: Type {};

    template <typename T>
    Form(Pos pos, const TType<T> &type, const T &data): pos(pos), type(type), data(data) {}

    template <typename T>
    T &as() { return any_cast<T &>(data); }

    template <typename T>
    const T &as() const { return any_cast<const T &>(data); }

    Pos pos;
    const Type &type;
    any data;
  };
}

#endif
