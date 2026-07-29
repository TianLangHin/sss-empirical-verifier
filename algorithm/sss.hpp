#ifndef SSS_HPP
#define SSS_HPP

#include <algorithm>
#include <compare>
#include <cstddef>
#include <functional>
#include <limits>
#include <list>
#include <stdexcept>
#include <vector>

#include "../theory/computation.hpp"
#include "../tree/node.hpp"

// This constant is used to define "positive infinity" as per the theoretical formulation.
constexpr static int infinity = std::numeric_limits<int>::max();

// Represents the two possible states that the second element of each tuple managed by SSS* can take on.
enum class Status { Live, Solved };

// This struct implements the tuples that the traversal frontier of SSS* will be composed of.
// It contains three elements: a tree node `v`, a traversal status `s`, and a merit value `h`.
// To make it more idiomatic to maintain an ordering over a collection of `Element` instances,
// thie struct also implements the `<=>` operator.
template <std::size_t Width, std::size_t Depth>
struct Element {
    Node<Width, Depth> v;
    Status s;
    int h;

    // An element `e1` is considered greater than `e2` (and will be nearer the front of a max priority queue)
    // if `e1.h > e2.h` or (`e1.h == e2.h` and `e1.v < e2.v`).
    // By Lemma A.3.2, `e1.v < e2.v` is true if and only if `L(e1.v) < L(e2.v)` or (`L(e1.v) == L(e2.v)` and `|e1.v| < |e2.v|`),
    // where `L(v)` is the value obtained by `v.get_left_rank()`.
    // Hence, the logic of the `<=>` operator for `Element` is composed of several smaller `<=>` operations
    // that are tiebroken by the next one (where the logic of some may be inverted).
    std::strong_ordering operator<=>(const Element& other) const {
        // Inverts the ordering given by some comparison returned by the `<=>` operator.
        // Effectively, this is the same as changing the logic from an expression `a <=> b` to that of `b <=> a`.
        auto invert = [](std::strong_ordering cmp) {
            return cmp == std::strong_ordering::less ? std::strong_ordering::greater
                : cmp == std::strong_ordering::greater ? std::strong_ordering::less
                : std::strong_ordering::equal;
        };

        // Takes in two results of the form `a1 <=> b1` and `a2 <=> b2`,
        // and returns the result that would be given by `(a1, a2) <=> (b1, b2)` by lexicographical comparison.
        auto tiebreak_on = [](std::strong_ordering first, std::strong_ordering second) {
            return first != std::strong_ordering::equal ? first : second;
        };

        // First, we calculate the three required comparisons.
        auto merit_cmp = h <=> other.h;
        auto left_rank_cmp = v.get_left_rank() <=> other.v.get_left_rank();
        auto length_cmp = v.get_length() <=> other.v.get_length();

        // They are finally composed as follows.
        return tiebreak_on(merit_cmp, invert(tiebreak_on(left_rank_cmp, length_cmp)));
    }
};

// Implements SSS* (Algorithm 5.1).
// Takes in the root node from which the search starts and the leaf heuristic function,
// as well as a dynamic array to keep track of the computation path taken and an integer to count how many leaf evaluations were made.
template <std::size_t Width, std::size_t Depth>
int sss(Node<Width, Depth> root, std::function<int(Node<Width, Depth>&)>& heuristic, std::vector<Computation>& computation_path, int& leaf_count) {

    // We start with the traversal frontier containing a single element.
    std::list<Element<Width, Depth>> open { { root, Status::Live, infinity } };

    // Main loop, applying the state space operator iteratively to the traversal frontier.
    while (!open.empty()) {
        // First, we record the size of the traversal frontier (used for annotating the computation path being tracked).
        std::size_t n = open.size();

        // Pop the top (highest-ranked) element from the traversal frontier.
        auto [v, s, h] = open.front();
        open.pop_front();

        if (!v.is_root() && s == Status::Solved) {
            if (v.get_length() % 2 == 1) {

                // Case 1 of the state space operator.
                Node<Width, Depth> parent_node = v.parent();
                open.remove_if([&](auto element) { return parent_node.is_ancestor_of(element.v); });
                open.push_front({ parent_node, Status::Solved, h });

                // We record the computation with the number of elements removed (by taking a difference of before and after),
                // as well as the original size of the traversal frontier.
                computation_path.push_back({ Case1 { .r = n - open.size(), .n = n } });

            } else if (v.get_length() % 2 == 0) {
                if (v.next().has_value()) {

                    // Case 2 of the state space operator.
                    open.push_front({ v.next().value(), Status::Live, h });

                    // We record the computation with the original size of the traversal frontier.
                    computation_path.push_back({ Case2 { .n = n } });

                } else {

                    // Case 3 of the state space operator.
                    open.push_front({ v.parent(), Status::Solved, h });

                    // We record the computation with the original size of the traversal frontier.
                    computation_path.push_back({ Case3 { .n = n } });
                }
            }
        } else if (s == Status::Live) {
            if (!v.first().has_value()) {

                // Case 4 of the state space operator.
                Element<Width, Depth> new_element { v, Status::Solved, std::min(heuristic(v), h) };
                auto insertion_location = std::find_if(open.begin(), open.end(), [&](auto element) { return new_element > element; });
                open.insert(insertion_location, new_element);

                // We record the computation with the original size of the traversal frontier,
                // the index at which the new element was inserted, and the number of infinite merit elements in the frontier.
                computation_path.push_back({
                    Case4 {
                        .n = n,
                        .j = static_cast<std::size_t>(std::distance(open.begin(), std::prev(insertion_location))),
                        .l = static_cast<std::size_t>(std::count_if(open.begin(), open.end(), [&](auto element) { return element.h == infinity; }))
                    }
                });
                // This is also the only case where a leaf node evaluation is conducted, so we increment the counter here.
                ++leaf_count;

            } else {
                if (v.get_length() % 2 == 1) {

                    // Case 5 of the state space operator.
                    open.push_front({ v.first().value(), Status::Live, h });

                    // We record the computation with the original size of the traversal frontier.
                    computation_path.push_back({ Case5 { .n = n } });

                } else if (v.get_length() % 2 == 0) {

                    // Case 6 of the state space operator.
                    for (std::size_t i = Width; i > 0; --i) {
                        open.push_front({ v.make_move(i), Status::Live, h });
                    }

                    // We record the computation with the original size of the traversal frontier.
                    computation_path.push_back({ Case6 { .n = n } });

                }
            }
        } else {

            // Case 7 of the state space operator, formalising the termination condition of the loop.
            computation_path.push_back({ Case7 { .n = n } });
            return h;
        }
    }

    // The original SSS* paper shows that this code path is not reachable,
    // and the algorithm will terminate via the `return` statement of Case 7.
    throw std::runtime_error("This is unreachable, since the state space operator must keep OPEN non-empty.");
}

#endif
