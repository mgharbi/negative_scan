#include <Halide.h>

namespace negascan {

using Halide::Generator;
using Halide::Var;
using Halide::Func;

class SubsamplerGenerator : public Generator<SubsamplerGenerator> {
public:
    Input<Buffer<int16_t>> input{"data", 3};
    Output<Buffer<int16_t>> output{"output", 3};

    void generate() {
      Var x("x"), y("y"), c("c");
      output(x, y, c) = input(x, y, c);
    }

};

}  // end namespace negascan

HALIDE_REGISTER_GENERATOR(
        negascan::SubsamplerGenerator, subsampler)
