#ifndef COMPUTATION_HPP
#define COMPUTATION_HPP

#include <cstddef>
#include <format>
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
// the number of spaces from the top the new element is inserted `j` (parameter `j` as per Section 5.1),
// and the number of elements with an infinite merit `l` (parameter `l` as per Section 5.1).
struct Case4 {
    std::size_t n;
    std::size_t j;
    std::size_t l;
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

// To allow the display of these computations for analysis,
// we define the following logic to make `Computation` instances printable via `std::format`.
template <class... Ts> struct overloaded : Ts... { using Ts::operator()...; };

template <>
struct std::formatter<Computation> {
    constexpr std::format_parse_context::iterator parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    FormatContext::iterator format(const Computation& computation, FormatContext& ctx) const {
        return std::visit(overloaded {
            [&](Case1 c) { return std::format_to(ctx.out(), "1({}, {})", c.r, c.n); },
            [&](Case2 c) { return std::format_to(ctx.out(), "2({})", c.n); },
            [&](Case3 c) { return std::format_to(ctx.out(), "3({})", c.n); },
            [&](Case4 c) { return std::format_to(ctx.out(), "4({}, {}, {})", c.n, c.j, c.l); },
            [&](Case5 c) { return std::format_to(ctx.out(), "5({})", c.n); },
            [&](Case6 c) { return std::format_to(ctx.out(), "6({})", c.n); },
            [&](Case7 c) { return std::format_to(ctx.out(), "7({})", c.n); }
        }, computation);
    }
};

#endif
