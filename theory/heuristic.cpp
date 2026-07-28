#include <algorithm>
#include <numeric>
#include <ranges>

#include "heuristic.hpp"
#include "util.hpp"

// Outputs the heuristic values (as per Definition 5.1) of all leaf nodes of the uniform game tree `G_{w, d}` in lexicographical order.
// Since the `left_distance` property ranks these leaf nodes uniquely and maps to the range `[0, w^d-1]`,
// the vector outputted by this function can be indexed directly by a node's `left_distance` to get the heuristic value.
std::vector<int> best_case(std::size_t width, std::size_t depth) {
    // The vector is pre-allocated, and the integer values for `w` and `d` are cast to `int` for calculation convenience.
    std::vector<int> values(pow(width, depth));

    int w = static_cast<int>(width);
    int d = static_cast<int>(depth);

    // We start at the first leaf node lexicographically: the node reached by always making Move 1.
    std::vector<int> node(depth, 1);

    for (int& value : values) {
        // First, we decompose our current node into `p` and `q`,
        // where `U(p, q)` recovers our current node by Definition 4.10.
        auto [p, q] = decision_sequences(node);

        // `optimal_p` is the vector representing the required structure of `p` given some `q`
        // such that the second line of the heuristic given by Definition 5.1 is satisfied.
        // The value `z` is the 1-based index of the first element in `q` that is not equal to 1.
        std::size_t z = std::distance(q.begin(), std::find_if(q.begin(), q.end(), [](int move) { return move != 1; })) + 1;
        std::vector<int> optimal_p(half_ceil(depth));
        for (std::size_t i = 0; i < half_ceil(depth); ++i) {
            optimal_p[i] = i < z ? w : 1;
        }

        // To identify which kind of value the node will take on,
        // we check the equalities of whether `p = {w}^ceil(d/2)` and `q = {1}^floor(d/2)`.
        bool p_is_all_w = std::all_of(p.begin(), p.end(), [=](int i) { return i == w; });
        bool q_is_all_1 = std::all_of(q.begin(), q.end(), [](int i) { return i == 1; });

        if ((p_is_all_w && q_is_all_1) || p == optimal_p) {
            // The first two cases of the expression in Definition 5.1.
            value = pow(w, half_ceil(d));
        } else if (!p_is_all_w && q_is_all_1) {
            // The third case of the expression in Definition 5.1.
            auto terms = std::views::iota(1, half_ceil(d) + 1)
                       | std::views::transform([&](int k) { return pow(w, half_ceil(d) - k) * (p[k-1] - 1); });
            value = 1 + std::reduce(terms.begin(), terms.end());
        } else {
            // The fourth case of the expression in Definition 5.1.
            // In this case, the value chosen is 0, although any other value can be chosen.
            value = 0;
        }

        // We move to the next leaf node by lexicographical order.
        next_sequence(node, w);
    }

    return values;
}

// Outputs the heuristic values (as per Definition 5.2) of all leaf nodes of the uniform game tree `G_{w, d}` in lexicographical order.
// Since the `left_distance` property ranks these leaf nodes uniquely and maps to the range `[0, w^d-1]`,
// the vector outputted by this function can be indexed directly by a node's `left_distance` to get the heuristic value.
std::vector<int> worst_case(std::size_t width, std::size_t depth) {
    // The vector is pre-allocated, and the integer values for `w` and `d` are cast to `int` for calculation convenience.
    std::vector<int> values(pow(width, depth));

    int w = static_cast<int>(width);
    int d = static_cast<int>(depth);

    // We start at the first leaf node lexicographically: the node reached by always making Move 1.
    std::vector<int> node(depth, 1);

    for (int& value : values) {
        // First, we decompose our current node into `p` and `q`,
        // where `U(p, q)` recovers our current node by Definition 4.10.
        auto [p, q] = decision_sequences(node);

        // The terms of the first summation in Definition 5.2 are generated in this iterator.
        auto p_terms = std::views::iota(1, half_ceil(d) + 1)
                     | std::views::transform([&](int k) { return pow(w, half_ceil(d) - k) * (p[k-1] - 1); });
        // The terms of the second summation in Definition 5.2 are generated in this iterator.
        auto q_terms = std::views::iota(1, half_floor(d) + 1)
                     | std::views::transform([&](int k) { return pow(w, half_floor(d) - k) * (q[k-1] - 1); });

        // The terms above are collected in a summation and used in the expression as per Definition 5.2.
        value = pow(w, d) - std::reduce(p_terms.begin(), p_terms.end()) - pow(w, half_ceil(d)) * std::reduce(q_terms.begin(), q_terms.end());

        // We move to the next leaf node by lexicographical order.
        next_sequence(node, w);
    }

    return values;
}

void next_sequence(std::vector<int>& sequence, int width) {
    std::size_t i = sequence.size() - 1;

    // First, we increment the right-most element,
    // as that is the first one that changes when moving rightward across leaf nodes.
    ++sequence[i];

    // We then carry any overflows (above `width`, which we take to be our numerical base) leftward
    // until there are no more such overflows, resetting an element to 1 whenever a carry is made.
    while (sequence[i] > width && i > 0) {
        sequence[i] = 1;
        ++sequence[--i];
    }
}

std::pair<std::vector<int>, std::vector<int>> decision_sequences(std::vector<int>& v) {
    std::vector<int> p {}, q {};
    // This loop guarantees that either `p` and `q` have the same length, or `p` is one element longer than `q`.
    // This abides by Definition 4.10.
    for (std::size_t i = 0; i < v.size(); ++i) {
        (i % 2 == 0 ? p : q).push_back(v[i]);
    }
    return { p, q };
}
