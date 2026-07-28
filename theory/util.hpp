#ifndef UTIL_HPP
#define UTIL_HPP

#include <concepts>

// Parity indicator function as per Definition 4.14.
template <std::integral T>
T parity(T x) {
    return x % 2;
}

// Function for dividing by 2 and flooring.
template <std::integral T>
T half_floor(T x) {
    return x / 2;
}

// Function for dividing by 2 and taking the ceiling.
template <std::integral T>
T half_ceil(T x) {
    return (x + 1) / 2;
}

// Exponentiation for integers using repeated squaring,
// since the standard library only provides implementations for floating-point numbers.
template <std::integral T>
T pow(T base, T exp) {
    if (exp == 0) {
        return 1;
    } else if (exp == 1) {
        return base;
    }
    int tmp = pow(base, exp / 2);
    return exp % 2 == 0 ? tmp * tmp : base * tmp * tmp;
}

#endif
