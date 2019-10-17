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

        Func input_f("input_f");
        input_f(c, x, y) = cast<float>(input(c, x, y)) / 65535.0f;

        // Correct for the film base color
        Func mask_correct("mask_correct");
        mask_correct(c, x, y) = input_f(c, x, y) / wp(c);

        Func film_gamma_correct("film_gamma_correct");
        film_gamma_correct(c, x, y) = pow(mask_correct(c, x, y), 1.0f/gamma(c));

        Func inverted("inverted");
        inverted(c, x, y) = 1.0f / film_gamma_correct(c, x, y);

        Func scaled("scaled");
        scaled(c, x, y) = clamp(exposure * (inverted(c, x, y) - 1.0f), 0.0f, 1.0f);

        // TODO: post inversion white balance

        Func corrected("corrected");
        corrected(c, x, y) = pow(scaled(c, x, y), 1.0f/output_gamma);

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
