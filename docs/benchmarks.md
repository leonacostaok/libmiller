# Benchmarks

macOS arm64, AppleClang 21, GMP 6.3.0, Release build. Mean CPU time over five runs.
Reproduce with `./build/miller-bench`.

| Input | Bits | Bases | Miller | GMP (`reps=25`) |
| --- | ---: | ---: | ---: | ---: |
| 2³¹ − 1 | 31 | 941 | 208 µs | 0.8 µs |
| 2⁶¹ − 1 | 61 | 3,646 | 1.44 ms | 1.6 µs |
| 2¹²⁷ − 1 | 127 | 15,806 | 48.7 ms | 4.6 µs |
| 341550071728321 (composite) | 49 | 22 | 7.2 µs | 1.4 µs |

GMP’s probable-prime results have a different guarantee. Small timings are noisy.
These measurements do not compare Miller with exact methods such as APR-CL or ECPP.
