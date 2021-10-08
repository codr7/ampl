#include <iostream>
#include <sstream>

#include "ampl/reader.hpp"
#include "ampl/vm.hpp"

using namespace std;
using namespace ampl;

void repl(VM &vm, istream &in, ostream &out) {
  out << endl <<
    "Press Return on empty line to evaluate." << endl << endl <<
    "May the Source be with you!" << endl << endl;
    
   stringstream buf;
  
   for (;;) {
     out << "  ";
     
     string line;
     if (!getline(in, line)) { break; }
       
     if (line.empty()) {
       deque<Form> fs;
       optional<Form> f;
       Pos pos("repl");
       try {
	 while ((f = read_form(buf, pos, vm))) { fs.push_back(*f); }
	 buf.str("");
	 buf.clear();
	 
	 PC start_pc = vm.pc();

	 vm.emit(fs);
	 vm.emit<ops::Stop>();
	 vm.eval(start_pc);
       } catch (const Error &e) {
	 out << e.what() << endl;
       }
       
       out << vm.stack() << endl;
     } else {
       buf << line << endl;
     }
   }
}

int main() {
  cout << "/>mpl v" << VERSION << endl;

  VM vm;
  vm.libs.abc.import();
  vm.libs.math.import();
  
  repl(vm, cin, cout);
  return 0;
}
