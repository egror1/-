#pragma once

namespace Roots {
enum class Method : int {
    Exit = 0,
    Iteration = 1,
    Bisection = 2,
    Newton = 3
};
enum class Status : int {
    Success,
    IterationLimit,
    ZeroDerivative,
    NonFinite,
    InvalidInterval,
    Stagnation
};

struct Result {
    double root;
    int iterations;
    Status status;
};

[[nodiscard]] double Evaluate(double x, double coefficient);
[[nodiscard]] Result Iterate(double coefficient, double tolerance, double initial);
[[nodiscard]] Result Bisect(double coefficient, double tolerance, double left, double right);
[[nodiscard]] Result Newton(double coefficient, double tolerance, double initial);
}  // namespace Roots
