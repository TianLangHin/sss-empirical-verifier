#ifndef NODE_HPP
#define NODE_HPP

#include <array>
#include <cassert>
#include <cstddef>
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
    // It indicates the last non-zero element of `position`, accessible by `position[length - 1]`.
    std::size_t length;
    // To support the Case 1 and Case 4 operations of SSS* using a constant number of integer operations,
    // we store the auxiliary values `ancestry` and `left_rank` respectively.
    // They satisfy the values defined in Lemma A.3.1 and Lemma A.3.2 respectively.
    uint64_t ancestry;
    uint64_t left_rank;

    constexpr static std::array<uint64_t, Depth + 1> power = []() {
        std::array<uint64_t, Depth + 1> arr {};
        uint64_t value = 1;
        for (std::size_t i = 0; i < Depth + 1; ++i) {
            arr[i] = value;
            value *= Width;
        }
        return arr;
    }();

public:
    // Default constructor to instantiate the root node. `position` is default-initialised to all zeros.
    Node(): position {}, length {0}, ancestry {0}, left_rank {0} {}

    Node(Position& position, std::size_t length, uint64_t ancestry, uint64_t left_rank):
        position {position}, length {length}, ancestry {ancestry}, left_rank {left_rank} {}

    Node parent() {
        // `parent()` will not be called on the root node.
        assert(length > 0);

        // `ancestry` and `left_rank` are updated as per the proof of Lemma A.3.3.
        // The indices used for `position` are one less than the 1-based notation of the main paper.
        uint64_t ancestry = this->ancestry - power[length - 1] * (position[length - 1] - 1);
        uint64_t left_rank = this->left_rank - power[Depth - length] * (position[length - 1] - 1);

        // One less space is used, thus that last space is set to zero.
        Position position {this->position};
        position[length - 1] = 0;

        // The parent node has a length one less than the original node.
        return Node(position, length - 1, ancestry, left_rank);
    }

    std::optional<Node> first() {
        // If the node is `d` spaces away from the root,
        // then it is a leaf node and has no first child node.
        if (length == Depth) {
            return std::nullopt;
        }

        // The space after our last non-zero element is set to 1 (first child).
        Position position {this->position};
        position[length] = 1;

        // The first child node has a length one more than the original node,
        // and both the `ancestry` and `left_rank` values remain the same.
        return Node(position, length + 1, ancestry, left_rank);
    }

    std::optional<Node> next() {
        // If the last move used to reach this node is `w`,
        // there are no more nodes to the right sharing the same parent.
        if (position[length - 1] == Width) {
            return std::nullopt;
        }

        // `ancestry` and `left_rank` are updated as per the proof of Lemma A.3.3.
        // The indices used for `position` are one less than the 1-based notation of the main paper.
        uint64_t ancestry = this->ancestry + power[length - 1];
        uint64_t left_rank = this->left_rank + power[Depth - length];

        // The last non-zero space is incremented.
        Position position {this->position};
        ++position[length - 1];

        // The next sibling node has the same length as the original node.
        return Node(position, length, ancestry, left_rank);
    }

    Node make_move(std::size_t i) {
        // A move can only be made from a non-leaf node,
        // and the only valid moves are the integers from `1` to `w` inclusive.
        assert(length < Depth);
        assert(1 <= i && i <= Width);

        // `ancestry` and `left_rank` are updated as per the proof of Lemma A.3.3.
        // The indices used for `position` are one less than the 1-based notation of the main paper.
        uint64_t ancestry = this->ancestry + power[length] * (i - 1);
        uint64_t left_rank = this->left_rank + power[Depth - length - 1] * (i - 1);

        // The space after our last non-zero element is set to `i`.
        Position position {this->position};
        position[length] = i;

        // The resultant node is a child node, and thus has a length one more than the original.
        return Node(position, length + 1, ancestry, left_rank);
    }

    bool is_ancestor_of(const Node& other) {
        // Comparison as per Lemma A.3.1.
        // The additional clause of `length < other.length` is added due to the assumption of Lemma A.3.1,
        // and that it is impossible for a longer node to be the ancestor of a shorter or equal-lengthed one.
        uint64_t base = power[length];
        return length < other.length && ancestry % base == other.ancestry % base;
    }

    bool is_root() {
        // The root node is uniquely the node that has a length of zero.
        return length == 0;
    }

    std::size_t get_length() const {
        return length;
    }

    Position& get_position() const {
        return position;
    }

    uint64_t get_left_rank() const {
        return left_rank;
    }
};

#endif
