# Miller Primality Testing

Deterministic primality testing with explicit mathematical guarantees.

[libmiller](https://github.com/leonacostaok/libmiller) provides:

- **C:** GMP-backed implementation and a decimal C interface.
- **Python:** typed bindings to the C library.
- **JavaScript / TypeScript:** native BigInt implementation, no runtime dependencies.

Full scans use the Miller/Bach bound and retain an explicit Dirichlet GRH dependency.
Experimental; not audited or formally verified. MIT licensed.

[Report an issue](https://github.com/leonacostaok/libmiller/issues/new/choose) ·
[Contribute](https://github.com/leonacostaok/libmiller/blob/main/CONTRIBUTING.md)

Mathematical review, test vectors, portability fixes and reproducible benchmarks are welcome.
