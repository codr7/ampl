#ifndef AMPL_FORM_HPP
#define AMPL_FORM_HPP

#include <deque>
#include <ostream>

#include "ampl/pos.hpp"

namespace ampl {
  using namespace std;

  struct VM;
  struct Form;
  
  namespace forms {
    using namespace ampl;
    
    template <typename T>
    void dump(const Form &self, const T &imp, ostream &out);

    template <typename T>
    void emit(const Form &self, const T &imp, deque<Form> &in, VM &vm) {}
  }

  struct Form {
    struct Imp {
      virtual ~Imp() = default;
      virtual void dump(const Form& self, ostream& out) const = 0;
      virtual void emit(const Form &self, deque<Form> &in, VM &vm) const = 0;
    };

    template <typename T>
    struct TImp: Imp {
      TImp(T imp): imp(move(imp)) { }

      void dump(const Form& self, ostream& out) const override {
	forms::dump(self, imp, out);
      }

      void emit(const Form& self, deque<Form> &in, VM &vm) const override {
	forms::emit(self, imp, in, vm);
      }
      
      T imp;
    };

    template <typename T>
    Form(Pos pos, T imp): pos(pos), imp(make_shared<TImp<T>>(move(imp))) {}

    template <typename T>
    const T& as() const { return dynamic_cast<const TImp<T>&>(*imp).imp; }

    template <typename T>
    const T* try_as() const {
      auto timp = dynamic_cast<const TImp<T> *>(imp.get());
      return timp ? &timp->imp : nullptr;
    }

    Pos pos;
    shared_ptr<const Imp> imp;
  };
}

#endif
