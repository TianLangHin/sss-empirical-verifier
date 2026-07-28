#include <iostream>

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

    return 0;
}
