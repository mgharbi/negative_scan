#include <Halide.h>

namespace negascan {

using Halide::Generator;
using Halide::Var;
using Halide::Func;

class MinMaxGenerator : public Generator<MinMaxGenerator> {
public:
    Input<Buffer<uint16_t>> input{"data", 3};
    Output<Buffer<float>> min_{"min", 1};
    Output<Buffer<float>> max_{"max", 1};

    RDom r;
    Func reduced;

    void generate() {
        Var x("x"), y("y"), c("c");

        Func input_f("input_f");
        input_f(c, x, y) = cast<float>(input(c, x, y)) / 65535.0f;

        RDom rchan(0, 3);

        Expr width = input.dim(1).extent();
        Expr height = input.dim(2).extent();

        reduced = Func("reduced");
        r = RDom(0, width, 0, height);
        reduced(c) = {input_f(c, 0, 0), input_f(c, 0, 0)};
        reduced(c) = {min(input_f(c, r.x, r.y), reduced(c)[0]), max(input_f(c, r.x, r.y), reduced(c)[1])};
        min_(c) = reduced(c)[0];
        max_(c) = reduced(c)[1];
    }

    void schedule() {
        Var x("x"), y("y"), c("c"), rf("rf");
        reduced
            .bound(c, 0, 3)
            .unroll(c, 3);
        Func f = reduced.update().rfactor(r.y, rf);
        f.parallel(rf);

    }

};

}  // end namespace negascan

HALIDE_REGISTER_GENERATOR(
        negascan::MinMaxGenerator, minmax)
