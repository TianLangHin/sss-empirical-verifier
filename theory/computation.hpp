#ifndef COMPUTATION_HPP
#define COMPUTATION_HPP

#include <cstddef>
#include <variant>

// Counts an instance of Case 1 of the SSS* state space operator being executed.
// Annotated with the number of elements removed `r` and the size of OPEN `n`.
struct Case1 {
    std::size_t r;
    std::size_t n;
    bool operator==(const Case1& other) const = default;
};

// Counts an instance of Case 2 of the SSS* state space operator being executed.
// Annotated with the size of OPEN `n`.
struct Case2 {
    std::size_t n;
    bool operator==(const Case2& other) const = default;
};

// Counts an instance of Case 3 of the SSS* state space operator being executed.
// Annotated with the size of OPEN `n`.
struct Case3 {
    std::size_t n;
    bool operator==(const Case3& other) const = default;
};

// Counts an instance of Case 4 of the SSS* state space operator being executed.
// Annotated with the size of OPEN `n`,
// the number of spaces from the top the new element is inserted `insertion_index` (parameter `j` as per Section 5.1),
// and the number of elements with an infinite merit `infinity_count` (parameter `l` as per Section 5.1).
struct Case4 {
    std::size_t n;
    std::size_t insertion_index;
    std::size_t infinity_count;
    bool operator==(const Case4& other) const = default;
};

// Counts an instance of Case 5 of the SSS* state space operator being executed.
// Annotated with the size of OPEN `n`.
struct Case5 {
    std::size_t n;
    bool operator==(const Case5& other) const = default;
};

// Counts an instance of Case 6 of the SSS* state space operator being executed.
// Annotated with the size of OPEN `n`.
struct Case6 {
    std::size_t n;
    bool operator==(const Case6& other) const = default;
};

// Counts an instance of Case 7 of the SSS* state space operator being executed.
// Annotated with the size of OPEN `n`.
struct Case7 {
    std::size_t n;
    bool operator==(const Case7& other) const = default;
};

// Since the computations of SSS* will be collected into a vector,
// we define `Computation` as an `std::variant` that can contain all seven types of computations.
typedef std::variant<Case1, Case2, Case3, Case4, Case5, Case6, Case7> Computation;

#endif
