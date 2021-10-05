#include <istream>
#include <vector>
#include "ampl/reader.hpp"

namespace ampl {
  using Reader = function<optional<Form> (istream &in, Pos &pos, VM &vm)>;
  
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

  optional<Form> read_form(istream &in, Pos &pos, VM &vm) {
    vector<Reader> readers {read_ws};

    for (Reader r: readers) {
      auto f = r(in, pos, vm);
      if (f) {
	return f;
      }
    }

    return nullopt;
  }
}
