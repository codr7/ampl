#ifndef AMPL_FORM_HPP
#define AMPL_FORM_HPP

#include <deque>
#include <optional>
#include <ostream>

#include "ampl/pos.hpp"
#include "ampl/val.hpp"

namespace ampl {
  using namespace std;

  struct VM;
  struct Form;
  
  namespace forms {
    using namespace ampl;
    
    template <typename T>
    void dump(const Form &self, const T &imp, ostream &out);

    template <typename T>
    void emit(const Form &self, const T &imp, deque<Form> &in, VM &vm);

    template <typename T>
    Val quote(const Form &self, const T &imp, VM &vm);

    template <typename T>
    optional<Val> val(const Form &self, const T &imp, VM &vm) { return nullopt; }
  }

  struct Form {
    struct Imp {
      virtual ~Imp() = default;
      virtual void dump(const Form& self, ostream& out) const = 0;
      virtual void emit(const Form &self, deque<Form> &in, VM &vm) const = 0;
      virtual Val quote(const Form &self, VM &vm) const = 0;
      virtual optional<Val> val(const Form &self, VM &vm) const = 0;
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

      Val quote(const Form &self, VM &vm) const override {
	return forms::quote(self, imp, vm);
      }

      optional<Val> val(const Form &self, VM &vm) const override {
	return forms::val(self, imp, vm);
      }

      T imp;
    };

    template <typename T>
    Form(const Pos &pos, T imp): pos(pos), imp(make_shared<TImp<T>>(move(imp))) {}

    template <typename T>
    const T& as() const { return dynamic_cast<const TImp<T>&>(*imp).imp; }

    template <typename T>
    bool is() const { return dynamic_cast<const TImp<T> *>(imp.get()); }

    void dump(ostream &out) const {
      imp->dump(*this, out);
    }

    void emit(deque<Form> &in, VM &vm) const {
      imp->emit(*this, in, vm);
    }

    Val quote(VM &vm) const{
      return imp->quote(*this, vm);
    }

    optional<Val> val(VM &vm) const{
      return imp->val(*this, vm);
    }

    Pos pos;
    shared_ptr<const Imp> imp;
  };

  ostream &operator <<(ostream &out, const Form &form);
}

#endif
