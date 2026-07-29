#include <chrono>
#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <utility>

#include "algorithm/alpha_beta.hpp"
#include "algorithm/sss.hpp"
#include "theory/computation.hpp"
#include "theory/heuristic.hpp"
#include "theory/paths.hpp"
#include "tree/node.hpp"

// The width and depth of the game tree must be supplied as arguments during compilation,
// since the tree's size parameters must be compile-time constants to allow tree nodes to be stack-allocated.
#ifndef WIDTH
#define WIDTH 1
#endif

#ifndef DEPTH
#define DEPTH 1
#endif

// Helper function to output the full hypothesised computation path
// in either the SSS* best case or the SSS* worst case
// when run on the given uniform game tree (as per the compile-time constants `Width` and `Depth`).
template <std::size_t Width, std::size_t Depth>
std::vector<Computation> theoretical_path(bool best) {
    std::vector<Computation> path {};
    phase_1<Width, Depth>(path);
    if (best) {
        phase_2<Width, Depth, true>(path);
        phase_3_best<Width, Depth>(path);
    } else {
        phase_2<Width, Depth, false>(path);
        phase_3_worst<Width, Depth>(path);
    }
    return path;
}

// Helper function to execute a function and time its runtime.
// Returns both the value outputted from the function and the time the function took.
std::pair<int, std::chrono::duration<double>> timed_call(std::function<int()>&& func) {
    auto start = std::chrono::steady_clock::now();
    int result = func();
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    return { result, elapsed };
}

// Helper function to run SSS* and alpha-beta pruning on either the best-case or worst-case configuration of the uniform game tree.
// It is parameterised by the compile-time template parameters `Width` and `Depth`,
// such that the algorithms are run on the uniform game tree `G_{w, d}`
// (where the subscripts `w` and `d` refer to the parameters `Width` and `Depth` respectively).
template <std::size_t Width, std::size_t Depth>
void test_case(std::function<int(Node<Width, Depth>&)>& heuristic, std::vector<Computation>& theory_path, std::optional<std::string> output_filename) {
    // First, the root of the game tree is instantiated.
    Node<Width, Depth> root {};

    // The SSS* algorithm is run on the uniform game tree, keeping track of the computation path taken and the number of leaf evaluations made.
    std::vector<Computation> actual_path {};
    int sss_leaf_count = 0;
    auto [sss_result, sss_elapsed] = timed_call(std::bind(sss<Width, Depth>, root, heuristic, std::ref(actual_path), std::ref(sss_leaf_count)));

    // Alpha-beta pruning is run on the uniform game tree, keeping track of the number of leaf evaluations made.
    int limit = std::numeric_limits<int>::max();
    int ab_leaf_count = 0;
    auto [ab_result, ab_elapsed] = timed_call(std::bind(alpha_beta<Width, Depth>, root, heuristic, -limit, limit, std::ref(ab_leaf_count)));

    std::cout << std::format("  SSS* Result: {0}, AB Result: {1}\n", sss_result, ab_result);
    std::cout << std::format("  SSS* Leaf Count: {0}, AB Leaf Count: {1}\n", sss_leaf_count, ab_leaf_count);
    std::cout << std::format("  SSS* Time: {0}, AB Time: {1}\n", sss_elapsed, ab_elapsed);
    std::cout << std::format("  Minimax Results Match: {0}\n", sss_result == ab_result);
    std::cout << std::format("  Leaf Count Domination (SSS* <= AB): {0}\n", sss_leaf_count <= ab_leaf_count);
    std::cout << std::format("  Computation Path Matches Theory: {0}\n", actual_path == theory_path);

    // If an output filename is given, the computation path of SSS* here is written to the file.
    if (output_filename.has_value()) {
        std::ofstream output_file(output_filename.value());
        for (Computation c : actual_path) {
            output_file << std::format("{0}\n", c);
        }
    }
}

int main(int argc, char* argv[]) {
    // If the argument "--write-file" is passed to the executable, then the computation paths of SSS* will be outputted.
    bool write_file = argc > 1 && static_cast<std::string>(argv[1]) == "--write-file";

    // The compile-time arguments are stored as the integer parameters they represent.
    constexpr std::size_t Width = WIDTH;
    constexpr std::size_t Depth = DEPTH;

    // The parameters of the uniform game tree must be positive.
    static_assert(Width > 0, "Tree width must be a positive integer.");
    static_assert(Depth > 0, "Tree depth must be a positive integer.");

    // To run the best case configuration, we precompute the leaf node heuristic values as per Definition 5.1,
    // bind that to the `heuristic` function, and compute the hypothesised SSS* best-case computation path.
    // The algorithms are then run in the configuration, and the results are outputted.
    std::function<int(Node<Width, Depth>&)> best_heuristic = std::bind(heuristic<Width, Depth>, best_case(Width, Depth), std::placeholders::_1);
    std::vector<Computation> best_theory_path = theoretical_path<Width, Depth>(true);
    std::optional<std::string> best_file = write_file ? std::make_optional(std::format("best_path_{0}_{1}.txt", Width, Depth)) : std::nullopt;
    std::cout << "Best Case:\n";
    test_case(best_heuristic, best_theory_path, best_file);

    // To run the worst case configuration, we precompute the leaf node heuristic values as per Definition 5.2,
    // bind that to the `heuristic` function, and compute the hypothesised SSS* worst-case computation path.
    // The algorithms are then run in the configuration, and the results are outputted.
    std::function<int(Node<Width, Depth>&)> worst_heuristic = std::bind(heuristic<Width, Depth>, worst_case(Width, Depth), std::placeholders::_1);
    std::vector<Computation> worst_theory_path = theoretical_path<Width, Depth>(false);
    std::optional<std::string> worst_file = write_file ? std::make_optional(std::format("worst_path_{0}_{1}.txt", Width, Depth)) : std::nullopt;
    std::cout << "Worst Case:\n";
    test_case(worst_heuristic, worst_theory_path, worst_file);

    return 0;
}
