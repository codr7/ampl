#ifndef AMPL_VAL_HPP
#define AMPL_VAL_HPP

#include  <any>
#include "ampl/type.hpp"

namespace ampl {
  using namespace std;

  struct VM;
  
  struct Val {
    template <typename T>
    Val(const TType<T> &type, const T& data): type(type), data(data) {}

    template <typename T>
    T &as() { return any_cast<T &>(data); }

    template <typename T>
    const T &as() const { return any_cast<const T &>(data); }

    void dump(ostream &out) const;

    bool is_true() const;
    
    Type type;
    any data;
  };

  bool operator==(const Val &lhs, const Val &rhs);
  ostream &operator <<(ostream &out, const Val &val);

  template <typename T>
  TType<T>::TType(const Sym &name): Type(name) {
    methods.dump = [](const Val &val, ostream &out) { out << val.as<T>(); };
    methods.is_equal = [](const Val &x, const Val &y) { return x.as<T>() == y.as<T>(); };
  }


}

#endif
