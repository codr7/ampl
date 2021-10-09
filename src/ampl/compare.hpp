#ifndef AMPL_COMPARE_HPP
#define AMPL_COMPARE_HPP

namespace ampl {
  enum Order {LT = -1, EQ = 0, GT = 1};

  template <typename T>
  Order compare(const T &x, const T &y);
}

#endif
