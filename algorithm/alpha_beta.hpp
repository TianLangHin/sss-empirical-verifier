#ifndef ALPHA_BETA_HPP
#define ALPHA_BETA_HPP

#include <algorithm>
#include <cstddef>
#include <functional>

#include "../tree/node.hpp"

// Conducts alpha-beta pruning on a uniform game tree `G_{w, d}` starting from the given `node`.
// Takes in a leaf heuristic function `heuristic` to evaluate leaf nodes,
// and its last argument keeps track of the total number of leaf node evaluations made.
template <std::size_t Width, std::size_t Depth>
int alpha_beta(Node<Width, Depth> node, std::function<int(Node<Width, Depth>&)>& heuristic, int alpha, int beta, int& leaf_count) {
    // A node belonging to the game tree `G_{w, d}` is a leaf node if its length is equal to `d`.
    if (node.get_length() == Depth) {
        ++leaf_count;
        int value = heuristic(node);
        // Since a negamax construct is used, the score is negated for the player opposite to the one playing at the root node.
        return node.get_length() % 2 == 0 ? value : -value;
    }
    // The fail-hard alpha-beta pruning construct is used here.
    int best_eval = alpha;
    for (std::size_t i = 1; i <= Width; ++i) {
        // In the negamax construct, the values for `alpha` and `beta` are negated and swapped when the player changes.
        best_eval = std::max(best_eval, -alpha_beta(node.make_move(i), heuristic, -beta, -best_eval, leaf_count));
        if (best_eval >= beta) {
            return beta;
        }
    }
    return best_eval;
}

#endif
