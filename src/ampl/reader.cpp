#include <istream>
#include <sstream>
#include <vector>
#include "ampl/forms/group.hpp"
#include "ampl/forms/id.hpp"
#include "ampl/forms/lit.hpp"
#include "ampl/reader.hpp"
#include "ampl/vm.hpp"

namespace ampl {
  optional<Form> read_form(istream &in, Pos &pos, VM &vm) {
    vector<Reader> readers {read_ws, read_int, read_group, read_id};
    optional<Form> f;
    
    for (Reader r: readers) {
      if ((f = r(in, pos, vm))) { break; }
    }

    return f;
  }

  optional<Form> read_group(istream &in, Pos &pos, VM &vm) {
    char c = 0;
    
    if (!in.get(c)) { return nullopt; }

    if (c != '(') {
      if (c) { in.unget(); }
      return nullopt;
    }

    Pos fpos = pos;
    deque<Form> body;
    
    for (;;) {
      read_ws(in, pos, vm);

      if (in.get(c)) {
	if (c == ')') { break; }
	in.unget();
      } else {
	break;
      }
      
      optional<Form> bf = read_form(in, pos, vm);
      if (!bf) { break; }
      body.push_back(*bf);
    }

    if (c != ')') { throw ReadError(fpos, "Open group"); }
    return Form(fpos, forms::Group(body));
  }

  optional<Form> read_id(istream &in, Pos &pos, VM &vm) {
    Pos fpos(pos);
    stringstream buf;
    char c = 0;
    
    while (in.get(c)) {
      if (!isgraph(c) || c == '(' || c == ')') {
	in.unget();
	break;
      }

      buf << c;
      pos.column++;
    }

    if (!buf.tellp()) { return nullopt; }
    return Form(fpos, forms::Id(vm.sym(buf.str())));
  }

   static int read_int_base(istream &in, Pos &pos, int base) {
    int v(0);
    
    static map<char, int8_t> char_vals = {
      {'0', 0}, {'1', 1}, {'2', 2}, {'3', 3}, {'4', 4}, {'5', 5}, {'6', 6}, {'7', 7},
      {'8', 8}, {'9', 9}, {'a', 10}, {'b', 11}, {'c', 12}, {'d', 13}, {'e', 14},
      {'f', 15}
    };
    
    auto ci(char_vals.end());
    char c = 0;
    
    while (in.get(c)) {
      if ((ci = char_vals.find(c)) == char_vals.end()) { break; }
      auto cv(ci->second);
      if (cv >= base) { throw ReadError(pos, "Invalid integer: ", c); }
      v = v * base + cv;
      pos.column++;
    }
    if (!in.eof()) { in.unget();}
    return v;
  }
  
  optional<Form> read_int(istream &in, Pos &pos, VM &vm) {
    char c = 0;

    if (!in.get(c)) { return nullopt; }
    if (c) { in.unget(); }
    if (!isdigit(c)) { return nullopt; }
    
    Pos fpos = pos;
    int v = read_int_base(in, pos, 10);
    return Form(fpos, forms::Lit(vm.libs.abc.int_type, v));
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
