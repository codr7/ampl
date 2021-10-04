#ifndef AMPL_POS_HPP
#define AMPL_POS_HPP

#include  <string>

namespace ampl {
  using namespace std;
  
  struct Pos {
    static const Pos INVALID;
    
    Pos(const string_view source = "n/a", int row = -1, int col = -1);

    string source;
    int row, col;
  };
}

#endif
