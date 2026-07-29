#ifndef PATHS_HPP
#define PATHS_HPP

#include <cassert>
#include <cstddef>
#include <vector>

#include "computation.hpp"
#include "util.hpp"

// All functions defined here are parameterised in their template by `W` and `D`,
// which correspond to the parameters of the uniform game tree `G_{w, d}`.
// They all take an additional argument `path`, which the theoretical computation path is inserted into.

// Phase 1 Computation Path (Algorithm 5.2).
template <std::size_t W, std::size_t D>
void phase_1(std::vector<Computation>& path) {
    for (std::size_t k = 1; k <= half_floor(D); ++k) {
        path.push_back({ Case6 { .n = 1 + (k - 1) * (W - 1) } });
        path.push_back({ Case5 { .n = 1 + k * (W - 1) } });
    }
    if (D % 2 == 1) {
        path.push_back({ Case6 { .n = 1 + half_floor(D) * (W - 1) } });
    }
    path.push_back({ Case4 { .n = 1 + half_ceil(D) * (W - 1), .j = half_ceil(D) * (W - 1), .l = half_ceil(D) * (W - 1) } });
}

// Branch Evaluation (Algorithm 5.3).
// Aside from the template parameters `W` and `D`,
// the `Best` parameter also determines if the best-case or worst-case version is being used.
// This computation path has a positive integer subscript `k`, which is its first argument.
template <std::size_t W, std::size_t D, bool Best>
void gamma(std::size_t k, std::size_t l, std::size_t n, std::vector<Computation>& path) {
    // `k` must be a positive integer.
    assert(k > 0);

    if (k == 1) {
        if (D % 2 == 0) {
            path.push_back({ Case5 { .n = n } });
        }
        path.push_back({ Case4 { .n = n, .j = Best ? l - 1 : n - 1, .l = l - 1 } });
    } else {
        path.push_back({ Case5 { .n = n } });
        path.push_back({ Case6 { .n = n } });
        for (std::size_t r = 1; r <= W; ++r) {
            gamma<W, D, Best>(k - 1, l + W - r, n + W - 1 + (r - 1) * (pow(W, k - 2) - 1), path);
        }
    }
}

// Phase 2 Computation Path (ALgorithm 5.4).
// Aside from the template parameters `W` and `D`,
// the `Best` parameter also determines if the best-case or worst-case version is being used.
template <std::size_t W, std::size_t D, bool Best>
void phase_2(std::vector<Computation>& path) {
    for (std::size_t k = 1; k <= half_ceil(D); ++k) {
        for (std::size_t j = 1; j <= W - 1; ++j) {
            gamma<W, D, Best>(k, 1 + (W - 1) * (half_ceil(D) - k + 1) - j, 1 + (W - 1) * (half_ceil(D) - k + 1) + j * (pow(W, k - 1) - 1), path);
        }
    }
}

// Subtree Solving (Algorithm 5.5).
// This computation path has a positive integer subscript `k`, which is its first argument.
template <std::size_t W, std::size_t D>
void lambda(std::size_t k, std::size_t n, std::vector<Computation>& path) {
    // `k` must be a positive integer.
    assert(k > 0);

    if (k == 1) {
        if (D % 2 == 0) {
            path.push_back({ Case4 { .n = n, .j = 0, .l = 0 } });
        } else if (D % 2 == 1) {
            path.push_back({ Case6 { .n = n } });
            path.push_back({ Case4 { .n = n + W - 1, .j = 0, .l = 0 } });
            path.push_back({ Case1 { .r = W - 1, .n = n + W - 1 } });
        }
    } else {
        path.push_back({ Case6 { .n = n } });
        path.push_back({ Case5 { .n = n + W - 1 } });
        for (std::size_t r = 1; r <= W - 1; ++r) {
            lambda<W, D>(k - 1, n + W - 1, path);
            path.push_back({ Case2 { .n = n + W - 1 } });
        }
        lambda<W, D>(k - 1, n + W - 1, path);
        path.push_back({ Case3 { .n = n + W - 1 } });
        path.push_back({ Case1 { .r = W - 1, .n = n + W - 1 } });
    }
}

// Phase 3 Best-Case Computation Path (Algorithm 5.6).
template <std::size_t W, std::size_t D>
void phase_3_best(std::vector<Computation>& path) {
    if (D % 2 == 1) {
        path.push_back({ Case1 { .r = W - 1, .n = pow(W, half_ceil(D)) } });
    }
    for (std::size_t k = 1; k <= half_floor(D); ++k) {
        for (std::size_t r = 1; r <= W - 1; ++r) {
            path.push_back({ Case2 { .n = pow(W, half_ceil(D)) - pow(W, parity(D) + k - 1) + 1 } });
            lambda<W, D>(k, pow(W, half_ceil(D)) - pow(W, parity(D) + k - 1) + 1, path);
        }
        path.push_back({ Case3 { .n = pow(W, half_ceil(D)) - pow(W, parity(D) + k - 1) + 1 } });
        path.push_back({
            Case1 {
                .r = pow(W, parity(D) + k - 1) * (W - 1),
                .n = pow(W, half_ceil(D)) - pow(W, parity(D) + k - 1) + 1
            }
        });
    }
    path.push_back({ Case7 { .n = 1 } });
}

// Subtree Expansion (Algorithm 5.7).
// This computation path has a positive integer subscript `k`, which is its first argument.
template <std::size_t W, std::size_t D>
void zeta(std::size_t k, std::size_t n, std::vector<Computation>& path) {
    // `k` must be a positive integer.
    assert(k > 0);

    if (k == 1) {
        if (D % 2 == 0) {
            path.push_back({ Case4 { .n = n, .j = n - 1, .l = 0 } });
        } else if (D % 2 == 1) {
            path.push_back({ Case6 { .n = n } });
            for (std::size_t r = 1; r <= W; ++r) {
                path.push_back({ Case4 { .n = n + W - 1, .j = n + W - 2, .l = 0 } });
            }
        }
    } else {
        path.push_back({ Case6 { .n = n } });
        for (std::size_t r = 1; r <= W; ++r) {
            path.push_back({ Case5 { .n = n + W - 1 + (r - 1) * (pow(W, parity(D) + k - 2) - 1) } });
            zeta<W, D>(k - 1, n + W - 1 + (r - 1) * (pow(W, parity(D) + k - 2) - 1), path);
        }
    }
}

// Subtree Expansion (Algorithm 5.8).
// This computation path has a positive integer subscript `k`, which is its first argument.
template <std::size_t W, std::size_t D>
void eta(std::size_t k, std::size_t n, std::vector<Computation>& path) {
    // `k` must be a positive integer.
    assert(k > 0);

    if (D % 2 == 1) {
        path.push_back({ Case1 { .r = W - 1, .n = n } });
    }
    for (std::size_t r = 1; r <= k - 1; ++r) {
        path.push_back({ Case3 { .n = n - pow(W, parity(D) + r - 1) + 1 } });
        path.push_back({ Case1 { .r = pow(W, parity(D) + r - 1) * (W - 1), .n = n - pow(W, parity(D) + r - 1) + 1 } });
    }
    path.push_back({ Case2 { .n = n - pow(W, parity(D) + k - 1) + 1 } });
}

// Subtree Shifting (Algorithm 5.9).
// This computation path has a positive integer subscript `k`, which is its first argument.
template <std::size_t W, std::size_t D>
void theta(std::size_t k, std::size_t n, std::vector<Computation>& path) {
    // `k` must be a positive integer.
    assert(k > 0);

    for (std::size_t j = 1; j <= W - 1; ++j) {
        for (std::size_t i = 1; i <= pow(W, half_floor(D) - k + 1); ++i) {
            eta<W, D>(k, n, path);
            zeta<W, D>(k, n - pow(W, parity(D) + k - 1) + 1, path);
        }
        for (std::size_t r = 1; r <= k - 1; ++r) {
            theta<W, D>(r, n, path);
        }
    }
}

// Phase 3 Best-Case Computation Path (Algorithm 5.10).
template <std::size_t W, std::size_t D>
void phase_3_worst(std::vector<Computation>& path) {
    for (std::size_t k = 1; k <= half_floor(D); ++k) {
        theta<W, D>(k, pow(W, half_ceil(D)), path);
    }
    if (D % 2 == 1) {
        path.push_back({ Case1 { .r = W - 1, .n = pow(W, half_ceil(D)) } });
    }
    for (std::size_t k = 1; k <= half_floor(D); ++k) {
        path.push_back({ Case3 { .n = pow(W, half_ceil(D)) - pow(W, parity(D) + k - 1) + 1 } });
        path.push_back({ Case1 { .r = pow(W, parity(D) + k - 1) * (W - 1), .n = pow(W, half_ceil(D)) - pow(W, parity(D) + k - 1) + 1 } });
    }
    path.push_back({ Case7 { .n = 1 } });
}

#endif
