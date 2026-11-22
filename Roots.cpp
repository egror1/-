#include "Roots.hpp"

#include <cmath>
#include <limits>
#include <numeric>

namespace {
const int kMaxIterations = 99999;
const double kHalfDivisor = 2.0;
const double kDerivativeThreshold = 16.0 * std::numeric_limits<double>::epsilon();

[[nodiscard]] bool OppositeSigns(const double first, const double second) {
    return std::signbit(first) != std::signbit(second);
}
}  // namespace

namespace Roots {
double Evaluate(const double x, const double coefficient) {
    return x - coefficient * std::cos(x);
}

Result Iterate(const double coefficient, const double tolerance, const double initial) {
    double current = initial;
    if (std::abs(Evaluate(current, coefficient)) <= tolerance) {
        return {.root = current, .iterations = 0, .status = Status::Success};
    }
    for (int iteration = 1; iteration <= kMaxIterations; ++iteration) {
        const double next = coefficient * std::cos(current);
        if (!std::isfinite(next)) {
            return {.root = current, .iterations = iteration, .status = Status::NonFinite};
        }
        const double residual = std::abs(Evaluate(next, coefficient));
        if (std::abs(next - current) <= tolerance && residual <= tolerance) {
            return {.root = next, .iterations = iteration, .status = Status::Success};
        }
        if (next == current) {
            return {.root = next, .iterations = iteration, .status = Status::Stagnation};
        }
        current = next;
    }
    return {.root = current, .iterations = kMaxIterations, .status = Status::IterationLimit};
}

Result Bisect(const double coefficient, const double tolerance, double left, double right) {
    double leftValue = Evaluate(left, coefficient);
    const double rightValue = Evaluate(right, coefficient);
    if (left >= right || !std::isfinite(leftValue) || !std::isfinite(rightValue)) {
        return {.root = left, .iterations = 0, .status = Status::InvalidInterval};
    }
    if (std::abs(leftValue) <= tolerance) {
        return {.root = left, .iterations = 0, .status = Status::Success};
    }
    if (std::abs(rightValue) <= tolerance) {
        return {.root = right, .iterations = 0, .status = Status::Success};
    }
    if (!OppositeSigns(leftValue, rightValue)) {
        return {.root = left, .iterations = 0, .status = Status::InvalidInterval};
    }
    for (int iteration = 1; iteration <= kMaxIterations; ++iteration) {
        const double middle = std::midpoint(left, right);
        const double middleValue = Evaluate(middle, coefficient);
        if (middleValue == 0.0 || (std::abs(middleValue) <= tolerance && (right - left) / kHalfDivisor <= tolerance)) {
            return {.root = middle, .iterations = iteration, .status = Status::Success};
        }
        if (middle == left || middle == right) {
            return {.root = middle, .iterations = iteration, .status = Status::Stagnation};
        }
        if (OppositeSigns(leftValue, middleValue)) {
            right = middle;
        } else {
            left = middle;
            leftValue = middleValue;
        }
    }
    return {.root = std::midpoint(left, right), .iterations = kMaxIterations, .status = Status::IterationLimit};
}

Result Newton(const double coefficient, const double tolerance, const double initial) {
    double current = initial;
    if (std::abs(Evaluate(current, coefficient)) <= tolerance) {
        return {.root = current, .iterations = 0, .status = Status::Success};
    }
    for (int iteration = 1; iteration <= kMaxIterations; ++iteration) {
        const double derivative = 1.0 + coefficient * std::sin(current);
        if (std::abs(derivative) <= kDerivativeThreshold) {
            return {.root = current, .iterations = iteration - 1, .status = Status::ZeroDerivative};
        }
        const double next = current - Evaluate(current, coefficient) / derivative;
        if (!std::isfinite(next)) {
            return {.root = current, .iterations = iteration, .status = Status::NonFinite};
        }
        if (std::abs(next - current) <= tolerance && std::abs(Evaluate(next, coefficient)) <= tolerance) {
            return {.root = next, .iterations = iteration, .status = Status::Success};
        }
        if (next == current) {
            return {.root = next, .iterations = iteration, .status = Status::Stagnation};
        }
        current = next;
    }
    return {.root = current, .iterations = kMaxIterations, .status = Status::IterationLimit};
}
}  // namespace Roots
