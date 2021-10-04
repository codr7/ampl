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
    Form(const TType<T> &type, Pos pos, const T &data): type(type), pos(pos), data(data) {}

    template <typename T>
    T &as() { return any_cast<T &>(data); }

    template <typename T>
    const T &as() const { return any_cast<const T &>(data); }

    const Type &type;
    Pos pos;
    any data;
  };
}

#endif
