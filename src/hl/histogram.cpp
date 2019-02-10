#include <Halide.h>

namespace negascan {

using Halide::Generator;
using Halide::Var;
using Halide::Func;

class HistogramGenerator : public Generator<HistogramGenerator> {
public:
    Input<Buffer<uint16_t>> input{"data", 3};
    Input<int> bins{"bins"};
    Output<Buffer<float>> output{"output", 2};

    RDom r;

    void generate() {
        Var x("x"), y("y"), c("c");

        Func input_f("input_f");
        input_f(c, x, y) = cast<float>(input(c, x, y)) / 65535.0f;

        Expr width = input.dim(1).extent();
        Expr height = input.dim(2).extent();

        Expr max_val = 1.0f;
        output(c, x) = 0.0f;
        r = RDom(0, width, 0, height);
        Expr bin = cast<int>(max(0, min(floor(input_f(c, r.x, r.y) * bins / max_val), bins-1)));
        output(c, bin) += 1.0f;
    }

    void schedule() {
        Var x("x"), y("y"), c("c"), rf("rf");
        output
            .bound(c, 0, 3)
            .unroll(c, 3);
    }

};

}  // end namespace negascan

HALIDE_REGISTER_GENERATOR(
        negascan::HistogramGenerator, histogram)
