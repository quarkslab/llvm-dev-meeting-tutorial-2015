#include "Utils.h"

// cl parser specialisation for the Ratio type
//
// See http://llvm.org/docs/CommandLine.html#extending-the-library

namespace llvm {
namespace cl {

// check if the user input is a real in [0., 1.]
bool parser<Ratio>::parse(Option &O, StringRef ArgName, const std::string &Arg, Ratio &Val) {
    char const* ArgCStr = Arg.c_str();
    char * EndPtr = nullptr;
    double TheRatio = std::strtod(ArgCStr, &EndPtr); // cannot use std::stod: no exception support in LLVM
    if(EndPtr == ArgCStr) {
      return O.error(ArgName + " value `" + Arg + "' is not a floating point value");
    }
    else if(0.> TheRatio or 1. < TheRatio) {
      return O.error("'" + Arg + "' is not in [0., 1.]");
    }
    else {
      Val.setRatio(TheRatio);
      // Quite surprisingly, returning false means no error happen...
      return false;
    }
  }

  void parser<Ratio>::printOptionDiff(const Option & O, Ratio const& , OptionValue<Ratio>,
                  size_t GlobalWidth) const {
    printOptionName(O, GlobalWidth);
  }
}

}

