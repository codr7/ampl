#include <iostream>
#include "ampl/ampl.hpp"
#include "ampl/forms/id.hpp"
#include "ampl/val.hpp"
#include "ampl/vm.hpp"

using namespace std;
using namespace ampl;

int main() {
  cout << "/>mpl v" << VERSION << endl;

  VM vm;
  Form form(Pos::INVALID, forms::Id(vm.sym("test_push")));
    
  vm.emit<ops::Push>(form, vm.libs.abc.int_type, 42);
  vm.emit<ops::Stop>();
  vm.eval(0);

  cout << vm.stack() << endl;
  return 0;
}
