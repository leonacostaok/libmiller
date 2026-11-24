# Mathematics

Under GRH for quadratic Dirichlet L-functions, every odd composite `n` has a
strong Miller witness at a base `a ≤ 2(ln n)²` (Miller/Bach).

For binary bit length `b`, `ln n < b ln 2 < 7b/10`. Therefore:

```text
2(ln n)² < 49b²/50
cutoff = min(n − 2, ceil(49b²/50))
```

The cutoff uses GMP integer arithmetic. Capping at `n − 2` is safe because every
composite has a proper divisor in `[2, n − 2]`, which is a failing base.

Write `n − 1 = d·2ˢ`, with `d` odd. A base passes if `aᵈ ≡ 1 (mod n)` or
`a^(d·2ʲ) ≡ −1 (mod n)` for some `0 ≤ j < s`. All integer bases from 2 through
the cutoff are tested. Failure proves compositeness independently of GRH;
full success invokes GRH. An interrupted scan establishes no primality result.

The November 2026 formalization includes Dirichlet GRH. This library has no
formal verification linking that theorem to its C implementation.

## References

- [Miller, Riemann’s Hypothesis and Tests for Primality](https://www.cs.cmu.edu/~glmiller/Publications/Papers/Mi75.pdf)
- Bach, Explicit bounds for primality testing and related problems, Math. Comp. 55 (1990), 355–380.
- [Schoof, Four primality testing algorithms, Theorem 2.2](https://pub.math.leidenuniv.nl/~stevenhagenp/ANTproc/05rene.pdf)
- [Dirichlet GRH formalization: verification record](https://math.mit.edu/~drew/grh/)
