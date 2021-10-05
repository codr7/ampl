#include "ampl/lib.hpp"
#include "ampl/vm.hpp"

namespace ampl {
  void Lib::import(VM &vm) {
    for (auto [k, v]: bindings) { vm.scope.bind(k, v); } 
  }

}
