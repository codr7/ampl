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

    Order compare(const Val &other) const;

    void dump(ostream &out) const;

    bool is_true() const;

    Type type;
    any data;
  };

  bool operator==(const Val &lhs, const Val &rhs);
  bool operator!=(const Val &lhs, const Val &rhs);
  ostream &operator <<(ostream &out, const Val &val);
}

#endif
