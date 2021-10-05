#ifndef AMPL_READER_HPP
#define AMPL_READER_HPP

#include <optional>
#include "ampl/form.hpp"

namespace ampl {
  optional<Form>read_id(istream &in, Pos &pos, VM &vm);
  optional<Form> read_ws(istream &in, Pos &pos, VM &vm);
  optional<Form> read_form(istream &in, Pos &pos, VM &vm);
}

#endif
