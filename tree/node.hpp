#ifndef NODE_HPP
#define NODE_HPP

#include <array>
#include <cassert>
#include <cstdint>
#include <optional>

// An instance of the `Node` class represents a node of the game tree `G_{w, d}`.
// To make a `Node` allocatable on the stack, `w` and `d` are made to be compile-time generic parameters.
template <std::size_t Width, std::size_t Depth>
class Node {
private:
    using Position = std::array<std::size_t, Depth>;

    // A node of the game tree `G_{w, d}` is identifiable by the sequence of moves used to reach it from the root.
    Position position;
    // The sequence of moves is stored in a fixed-sized array,
    // but only the first `l` are considered if it is `l` moves from the root.
    std::size_t length;
    // To support the Case 1 and Case 4 operations of SSS* using a constant number of integer operations,
    // we store the auxiliary values `ancestry` and `left_rank` respectively.
    // They satisfy the values defined in Lemma A.3.1 and Lemma A.3.2 respectively.
    uint64_t ancestry;
    uint64_t left_rank;

public:
    // Default constructor to instantiate the root node. `position` is default-initialised to all zeros.
    Node(): position {}, length {0}, ancestry {0}, left_rank {0} {}

    Node(Position& position, std::size_t length, uint64_t ancestry, uint64_t left_rank):
        position {position}, length {length}, ancestry {ancestry}, left_rank {left_rank} {}

    Node parent() {
        assert(length > 0);

        Position position {this->position};
        position[length - 1] = 0;

        return Node(position, length - 1, ancestry, left_rank);
    }

    std::optional<Node> first() {
        if (length == Depth) {
            return std::nullopt;
        }

        Position position {this->position};
        position[length] = 1;

        return Node(position, length + 1, ancestry, left_rank);
    }

    std::optional<Node> next() {
        if (position[length - 1] == Width) {
            return std::nullopt;
        }

        Position position {this->position};
        ++position[length - 1];

        return Node(position, length, ancestry, left_rank);
    }

    Node make_move(std::size_t i) {
        assert(length < Depth);
        assert(1 <= i && i <= Width);

        Position position {this->position};
        position[length] = i;

        return Node(position, length + 1, ancestry, left_rank);
    }
};

#endif
