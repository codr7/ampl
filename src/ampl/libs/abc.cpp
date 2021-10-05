#include "ampl/libs/abc.hpp"
#include "ampl/type.hpp"
#include "ampl/vm.hpp"

#include <ostream>

namespace ampl::libs {
  ABC::ABC(VM &vm):
    bool_type(vm.sym("Bool")),
    int_type(vm.sym("Int")),
    stack_type(vm.sym("Stack")) {
    bool_type.methods.dump = [](auto &val, auto &out) { out << (val.template as<bool>() ? 'T' : 'F'); };
    bool_type.methods.is_true = [](auto &val) { return val.template as<bool>(); };
    int_type.methods.is_true = [](auto &val) { return val.template as<int>(); };
    stack_type.methods.is_true = [](auto &val) { return !val.template as<Stack>().empty(); };
  }
}
