#include <Halide.h>

namespace negascan {

using Halide::Generator;
using Halide::Var;
using Halide::Func;

class InvertNegativeGenerator : public Generator<InvertNegativeGenerator> {
public:
    GeneratorParam<bool> bw{"bw", false};
    Input<Buffer<uint16_t>> input{"data", 3};
    Input<Buffer<float>> camera_rgb{"camera_rgb", 2};
    Input<Buffer<float>> gamma{"gamma", 1};
    Input<Buffer<float>> wp{"wp", 1};
    Input<Buffer<float>> bp{"bp", 1};
    Input<float> exposure{"exposure"};
    Input<float> output_gamma{"output_gamma"};
    Output<Buffer<uint16_t>> output{"output", 3};

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

        Func scaled("scaled");
        scaled(c, x, y) = pow(
                clamp(exposure * (inverted(c, x, y) - bp(c)) / (wp(c) - bp(c)), 0.0f, 1.0f), gamma(c));

        Func corrected("corrected");
        corrected(c, x, y) = 
                pow(scaled(c, x, y), 1.0f/output_gamma);

        if (bw) {
            output(c, x, y) = cast<uint16_t>(65535 * clamp(corrected(1, x, y), 0.0f, 1.0f));
        } else {
            output(c, x, y) = cast<uint16_t>(65535 * clamp(corrected(c, x, y), 0.0f, 1.0f));
        }
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
        negascan::InvertNegativeGenerator, invert_negative)
