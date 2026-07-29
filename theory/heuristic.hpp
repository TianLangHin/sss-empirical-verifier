#ifndef HEURISTIC_HPP
#define HEURISTIC_HPP

#include <cstddef>
#include <utility>
#include <vector>

#include "../tree/node.hpp"

// These two functions take in the parameters `w` and `d` of the game tree `G_{w, d}`
// and output a vector of size `w^d` containing the leaf heuristic values for all leaf nodes
// of the tree in increasing lexicographical order.
std::vector<int> best_case(std::size_t width, std::size_t depth);
std::vector<int> worst_case(std::size_t width, std::size_t depth);

// The outputs of the above functions can be passed as the first parameter of this function,
// using the `left_rank` value (defined as per Lemma A.3.2) of a node as its index in the vector.
template <std::size_t Width, std::size_t Depth>
int heuristic(std::vector<int>& values, Node<Width, Depth>& node) {
    return values[node.get_left_rank()];
}

// Modifies a given vector to the next lexicographically larger sequence,
// where every element of the sequence is an integer from `1` to `width`.
void next_sequence(std::vector<int>& sequence, int width);

// Decomposes some tree node (`v`) into the sequences `p` and `q`,
// where `U(p, q) = v` as per Definition 4.10.
std::pair<std::vector<int>, std::vector<int>> decision_sequences(std::vector<int>& v);

#endif
