#ifndef AMPL_POS_HPP
#define AMPL_POS_HPP

#include  <string>

namespace ampl {
  using namespace std;
  
  struct Pos {
    static const int MIN_LINE = 1, MIN_COLUMN = 0;
    static const Pos INVALID;
    
    Pos(const string &source = "*invalid*", int line = MIN_LINE, int column = MIN_COLUMN):
      source(source), line(line), column(column) {}

    string source;
    int line, column;
  };

  ostream &operator <<(ostream &out, const Pos &pos);
}

#endif
