#include "ampl/libs/abc.hpp"
#include "ampl/type.hpp"
#include "ampl/vm.hpp"

#include <ostream>

namespace ampl::libs {
  ABC::ABC(VM &vm):
    int_type(vm.sym("Int")),
    stack_type(vm.sym("Stack")) {}
}
