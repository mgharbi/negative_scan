#include <Halide.h>

namespace negascan {

using Halide::Generator;
using Halide::Var;
using Halide::Func;

class SubsampleGenerator : public Generator<SubsampleGenerator> {
public:
    Input<Buffer<uint16_t>> input{"data", 3};
    Input<int> width{"width"};
    Input<int> height{"height"};
    Output<Buffer<uint16_t>> output{"output", 3};

    void generate() {
        Var x("x"), y("y"), c("c");

        Expr iwidth = input.dim(1).extent();
        Expr iheight = input.dim(2).extent();

        Expr x_in = cast<int>(iwidth*((x + 0.5f) / width));
        Expr y_in = cast<int>(iheight*((y + 0.5f) / height));

        output(c, x, y) = input(c, x_in, y_in);
    }

    void schedule() {
        Var x("x"), y("y"), c("c");
        output
            .bound(c, 0, 3)
            .unroll(c, 3)
            .parallel(y, 8)
            .vectorize(x, 8);
    }

};

}  // end namespace negascan

HALIDE_REGISTER_GENERATOR(
        negascan::SubsampleGenerator, subsample)
