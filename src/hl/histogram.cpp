#include <Halide.h>

namespace negascan {

using Halide::Generator;
using Halide::Var;
using Halide::Func;

class HistogramGenerator : public Generator<HistogramGenerator> {
public:
    Input<Buffer<uint16_t>> input{"data", 3};
    Input<Buffer<float>> camera_rgb{"camera_rgb", 2};
    Input<int> bins{"bins"};
    Output<Buffer<float>> output{"output", 2};

    RDom r;

    void generate() {
        Var x("x"), y("y"), c("c");

        Expr eps = 1.0f / 65535.0f;
        Func input_f("input_f");
        input_f(c, x, y) = cast<float>(input(c, x, y)) / 65535.0f;

        RDom rchan(0, 3);
        Func rgb("rgb");
        rgb(c, x, y) = 0.0f;
        rgb(c, x, y) += camera_rgb(rchan, c)*input_f(rchan, x, y);

        Func inverted("inverted");
        inverted(c, x, y) = 1.0f /  max(rgb(c, x, y),  eps);;

        Expr width = input.dim(1).extent();
        Expr height = input.dim(2).extent();

        Expr max_val = 200.0f;
        output(c, x) = 0.0f;
        r = RDom(0, width, 0, height);
        Expr bin = cast<int>(max(0, min(floor(inverted(c, r.x, r.y) * bins / max_val), bins-1)));
        output(c, bin) += 1.0f;
    }

    void schedule() {
        Var x("x"), y("y"), c("c"), rf("rf");
        output
            .bound(c, 0, 3)
            .unroll(c, 3);
        // Func f = output.update().rfactor(r.y, rf);
        // f.parallel(rf);

    }

};

}  // end namespace negascan

HALIDE_REGISTER_GENERATOR(
        negascan::HistogramGenerator, histogram)
