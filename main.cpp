#include <functional>
#include <iostream>
#include <limits>

#include "algorithm/alpha_beta.hpp"
#include "algorithm/sss.hpp"
#include "theory/computation.hpp"
#include "theory/heuristic.hpp"
#include "theory/paths.hpp"
#include "tree/node.hpp"

int main() {
    std::cout << "SSS* Empirical Verifier Program.\n";

    Node<3, 3> root {};
    auto n1 = root.make_move(1).make_move(2).make_move(3);
    std::cout << (n1.first().has_value() == false) << '\n';
    auto n2 = n1.parent().parent().next();
    std::cout << (n2.has_value() == true) << '\n';
    auto n3 = n2.value().next().value().next();
    std::cout << (n3.has_value() == false) << '\n';

    std::vector<int> best = best_case(3, 3);
    std::vector<int> worst = worst_case(3, 3);

    for (int i = 0; i < 27; ++i) {
        std::cout << i << ' ' << best[i] << ' ' << worst[i] << '\n';
    }

    Computation c1 { Case1 { .r = 1, .n = 3 } };
    Computation c2 { Case1 { .r = 1, .n = 3 } };
    std::cout << (c1 == c2) << '\n';
    Computation c3 { Case5 { .n = 10 } };
    Computation c4 { Case6 { .n = 10 } };
    std::cout << (c3 == c4) << '\n';

    std::vector<Computation> path {};
    phase_1<4, 4>(path);
    phase_2<4, 4, false>(path);
    phase_3_worst<4, 4>(path);

    for (auto c : path) {
        std::cout << std::format("{} ", c);
    }
    std::cout << '\n';

    Node<4, 4> node {};
    int limit = std::numeric_limits<int>::max();
    int leaf_count = 0;
    std::vector<int> worst_values = worst_case(4, 4);
    std::function<int(Node<4, 4>&)> worst_heuristic = std::bind(heuristic<4, 4>, worst_values, std::placeholders::_1);

    int ab_result = alpha_beta(node, worst_heuristic, -limit, limit, leaf_count);
    std::cout << "Result: " << ab_result << " Leaf Count: " << leaf_count << '\n';

    std::vector<Computation> sss_path {};
    int sss_leaf_count = 0;
    int sss_result = sss(node, worst_heuristic, sss_path, sss_leaf_count);
    std::cout << "Result: " << sss_result << " Leaf Count: " << sss_leaf_count << '\n';
    std::cout << "Matching Computations: " << (path == sss_path) << '\n';

    return 0;
}
