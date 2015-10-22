#ifndef LLVMDEMO_UTILS_H
#define LLVMDEMO_UTILS_H

/* for cl option registration
 * {
 */
#include "llvm/Support/CommandLine.h"

// A type holder for a ratio in [0., 1.] interval
class Ratio {

  double Value_;

public:
  Ratio() : Value_(0.) {}
  Ratio(double Value) : Value_(Value) {}
  double getRatio() const { return Value_; }
  void setRatio(double Value) { Value_ = Value; }
};

// cl parser specialisation to parse Ratio from the command line
namespace llvm {
namespace cl {

template <> class parser<Ratio> : public basic_parser<Ratio> {
public:
  parser(Option &O) : basic_parser<Ratio>(O) {}
  virtual ~parser(){};

  bool parse(Option &O, StringRef ArgName, const std::string &Arg, Ratio &Val);

  void printOptionDiff(const Option &O, Ratio const &V, OptionValue<Ratio> D,
                       size_t GlobalWidth) const;
  void anchor() override {}
};
}
}

// The random number generator bundled with LLVM is not compatible with <random>
// (bug opened!)
// Provide the relevant wrapper here
#include "llvm/Support/RandomNumberGenerator.h"
namespace compat {

class RandomNumberGenerator {
  using generator_type = std::mt19937_64;
  std::unique_ptr<llvm::RandomNumberGenerator> RNG;

public:
  using result_type = generator_type::result_type;
  RandomNumberGenerator() = default;
  RandomNumberGenerator(llvm::RandomNumberGenerator *RNG) : RNG(RNG) {}

  generator_type::result_type operator()() { return (*RNG)(); }
  static constexpr generator_type::result_type min() {
    return generator_type::min();
  }
  static constexpr generator_type::result_type max() {
    return generator_type::max();
  }
};
}
#endif
