#ifndef AMPL_OP_HPP
#define AMPL_OP_HPP

#include "ampl/form.hpp"
#include "ampl/types.hpp"

namespace ampl {
  using namespace std;

  struct VM;

  enum Code {AMPL_GOTO = 0, AMPL_PUSH, AMPL_STOP};

  struct Op {
    static const int DATA_SIZE = 64;
    
    template <typename T>
    Op(const Form &form, const T &data): form(form), code(T::code) {
      assert(sizeof(T) <= DATA_SIZE);
      reinterpret_cast<T &>(data) = data;
    }

    template <typename T>
    T &as() { return any_cast<T &>(data); }

    template <typename T>
    const T &as() const { return any_cast<const T &>(data); }

    Form form;
    Code code;
    typename std::aligned_storage<DATA_SIZE, alignof(max_align_t)>::type data;
  };
}

#endif
