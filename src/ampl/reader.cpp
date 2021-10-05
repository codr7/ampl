#include <istream>
#include <sstream>
#include <vector>
#include "ampl/forms/id.hpp"
#include "ampl/reader.hpp"
#include "ampl/vm.hpp"

namespace ampl {
  optional<Form> read_form(istream &in, Pos &pos, VM &vm) {
    vector<Reader> readers {read_ws, read_id};
    optional<Form> f;
    
    for (Reader r: readers) {
      if ((f = r(in, pos, vm))) { break; }
    }

    return f;
  }

  optional<Form> read_id(istream &in, Pos &pos, VM &vm) {
    Pos fpos(pos);
    stringstream buf;
    char c = 0;
    
    while (in.get(c)) {
      if (!isgraph(c)) {
	in.unget();
	break;
      }

      buf << c;
      pos.column++;
    }

    if (!buf.tellp()) { return nullopt; }
    return Form(fpos, forms::Id(vm.sym(buf.str())));
  }
  
  optional<Form> read_ws(istream &in, Pos &pos, VM &vm) {
    char c = 0;
    
    while (in.get(c)) {
      if (!isspace(c)) {
	in.unget();
	break;
      }
	  
      switch (c) {
      case ' ':
      case '\t':
	pos.column++;
        break;
      case '\n':
        pos.line++;
        pos.column = Pos::MIN_COLUMN;
	break;
      };
    }
    
    return nullopt;
  }

}
