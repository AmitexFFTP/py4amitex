#ifndef _PMVOX_HEADER_
#define _PMVOX_HEADER_

#include "amitex/input.hpp"

amitex::Ptr<amitex::Input> make_thermo_pmvox_input(std::string_view resultsDir = "amitex_results0");
amitex::Ptr<amitex::Input> make_thermo_pmvox_input_gen(std::string_view resultsDir, int nmat);

#endif  //  _PMVOX_HEADER_
