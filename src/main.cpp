#include <iostream>
#include "ampl/forms/id.hpp"
#include "ampl/val.hpp"
#include "ampl/vm.hpp"

using namespace std;
using namespace ampl;

int main() {
  cout << "Welcome to />mpl v1" << endl;

  VM vm;
  Form form = forms::make_id(Pos::INVALID, vm.sym("test_push"));
    
  vm.emit(ops::make_push(form, vm.libs.abc.int_type, 42));
  vm.emit(ops::stop());
  vm.eval(0);

  cout << vm.stack << endl;
  return 0;
}
