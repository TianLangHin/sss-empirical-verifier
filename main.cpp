#include <iostream>

#include "theory/heuristic.hpp"
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

    return 0;
}
