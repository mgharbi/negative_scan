#include <Halide.h>

namespace negascan {

using Halide::Generator;
using Halide::Var;
using Halide::Func;

class InvertNegativeGenerator : public Generator<InvertNegativeGenerator> {
public:
    GeneratorParam<bool> bw{"bw", false};
    Input<Buffer<uint16_t>> input{"data", 3};
    Input<Buffer<float>> gamma{"gamma", 1};
    Input<Buffer<float>> wp{"wp", 1};
    Input<float> exposure{"exposure"};
    Input<float> bp{"bp"};
    Input<float> output_gamma{"output_gamma"};
    // Input<bool> bp{"bool"};
    Output<Buffer<uint16_t>> output{"output", 3};

    void generate() {
        Var x("x"), y("y"), c("c");

        Expr eps = 1.5259021896696422e-05f;

        Func input_f("input_f");
        input_f(c, x, y) = cast<float>(input(c, x, y)) / 65535.0f;

        Func wp_corrected("wp_corrected");
        wp_corrected(c, x, y) = min(input_f(c, x, y) / wp(c) + eps, 1.0f);

        Func positive("positive");
        positive(c, x, y) = 0.1f * max(1.0f / wp_corrected(c, x, y) - 1.0f, 0.0f);

        Func film_gamma("film_gamma");
        // film_gamma(c, x, y) = pow(positive(c, x, y), gamma(c));
        film_gamma(c, x, y) = pow(positive(c, x, y), 1.0f / gamma(c));

        Func corrected("corrected");
        corrected(c, x, y) = 
                pow(exposure * film_gamma(c, x, y), 1.0f/output_gamma);

        if (bw) {
            output(c, x, y) = cast<uint16_t>(65535 * clamp(corrected(1, x, y), 0.0f, 1.0f));
        } else {
            output(c, x, y) = cast<uint16_t>(65535 * clamp(corrected(c, x, y), 0.0f, 1.0f));
        }
    }

    void schedule() {
        Var x("x"), y("y"), c("c");
        output
            .parallel(y, 8)
            .vectorize(x, 8);
    }

};

}  // end namespace negascan

HALIDE_REGISTER_GENERATOR(
        negascan::InvertNegativeGenerator, invert_negative)
