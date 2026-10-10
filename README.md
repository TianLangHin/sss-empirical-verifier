# SSS* Algorithm Computation Path Verifier

This program runs the SSS* algorithm developed by [Stockman (1979)](https://doi.org/10.1016/0004-3702(79)90016-X)
on a uniform game tree $G_{w, d}$ of width $w$ and depth $d$
to trace out its computation path in the best case and worst case.
For a given pair of values $(w, d)$,
the program empirically verifies the correctness of the theoretical description of the algorithm's computation path
in the best case and worst case when the input game tree has width $w$ and depth $d$.
It also verifies its dominance over alpha-beta pruning in terms of the number of leaf node evaluations.

## Usage

Since each individual node of the game tree is represented as a stack-allocated array of size $d$,
the program requires both $w$ and $d$ to be passed in at compile time before running the empirical computation path verification.

The running script (either `run.cmd` for Windows or `run.sh` for Linux and Mac) takes $w$ and $d$ as two mandatory arguments,
as well as an optional third argument to output the actual computation path taken by the algorithm.
It compiles the C++ executable setting the input game tree parameters $w$ and $d$ to the values of `<width>` and `<depth>` respectively.
If the `--write-file` argument is provided,
the best-case and worst-case computation paths of the algorithm for this tree size are outputted into text files.
Both $w$ and $d$ need to be integers at least 1.

### For Windows

```
run.cmd <width> <depth> [--write-file]
```

### For Linux and Mac

```
./run.sh <width> <depth> [--write-file]
```
