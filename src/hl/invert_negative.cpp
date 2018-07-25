#include <Halide.h>

namespace negascan {

using Halide::Generator;
using Halide::Var;
using Halide::Func;

class InvertNegativeGenerator : public Generator<InvertNegativeGenerator> {
public:
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

        Expr eps = 1e-4f;

        Func input_f("input_f");
        input_f(c, x, y) = cast<float>(input(c, x, y)) / 65535.0f;

        Func wp_corrected("wp_corrected");
        wp_corrected(c, x, y) = min((input_f(c, x, y) + eps) / wp(c), 1.0f);

        Func positive("positive");
        positive(c, x, y) = max(1.0f / wp_corrected(c, x, y) - 1.0f - bp, 0.0f);

        Func film_gamma("film_gamma");
        // film_gamma(c, x, y) = pow(positive(c, x, y), gamma(c));
        film_gamma(c, x, y) = pow(positive(c, x, y), 1.0f / gamma(c));

        // output(c, x, y) = cast<uint16_t>(
        //         65535 * clamp(pow(exposure * film_gamma(c, x, y), output_gamma), 0.0f, 1.0f));
        output(c, x, y) = cast<uint16_t>(
                65535 * clamp(pow(exposure * film_gamma(c, x, y), 1.0f/output_gamma), 0.0f, 1.0f));
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
