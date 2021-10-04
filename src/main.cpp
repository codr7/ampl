#include <iostream>
#include "ampl/val.hpp"
#include "ampl/vm.hpp"

using namespace std;
using namespace ampl;

int main() {
  cout << "Welcome to />mpl v1" << endl;

  VM vm;
  
  vm.push(vm.libs.abc.int_type, 42);
  cout << vm.stack << endl;
  
  return 0;
}
