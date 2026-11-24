# libmiller

Minimal implementations of Miller’s primality test for C, Python and JavaScript/TypeScript.

- Deterministic base selection with an integer-only cutoff.
- Explicit GRH dependency; no probable-prime results.
- Work limits, cancellation and compositeness witnesses.
- Shared/static library and pkg-config support.

| Implementation | Dependency | API |
| --- | --- | --- |
| [C](include/miller.h) | GMP | `miller_check`, `miller_check_decimal` |
| [Python](python/README.md) | C shared library | `check_prime` |
| [JavaScript / TypeScript](javascript/README.md) | Native BigInt | `checkPrime` |

## Build

Requires GMP, pkg-config, CMake ≥ 3.16 and a C99 compiler.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
cmake --install build --prefix "$PWD/install"
```

Link with `pkg-config --cflags --libs libmiller`.
Use `-DBUILD_SHARED_LIBS=OFF` for a static library.

## Usage

```c
#include <miller.h>

int main(void) {
    mpz_t n;
    mpz_init_set_str(n, "65537", 10);
    miller_options options = { .max_bases = 10000 };
    miller_result result = miller_check(n, &options, NULL);
    mpz_clear(n);
    return result.status == MILLER_PRIME_GRH ? 0 : 1;
}
```

`miller_check(n, options, witness)` returns a status and the number of bases tested.
`options` and `witness` may be `NULL`. A zero budget means unlimited.

| Status | Meaning |
| --- | --- |
| `MILLER_NOT_PRIME` | Not prime; includes integers below 2 |
| `MILLER_PRIME` | Recognized small prime; independent of GRH |
| `MILLER_PRIME_GRH` | Full scan passed; primality follows from Dirichlet GRH |
| `MILLER_INCOMPLETE` | Budget or cancellation stopped the scan |
| `MILLER_INVALID` | Invalid decimal input or output buffer |

**Do not interpret the status as a boolean.**

`miller_check_decimal(text, max_bases, &tested, witness, size)` provides a C interface
without GMP objects. Input accepts an optional minus sign followed by ASCII digits.
The optional witness buffer needs at least `strlen(text) + 1` bytes. FFI ABI version: 1.

`witness`, if supplied, must be initialized and distinct from `n`. It receives a
proper divisor or failing Miller base, otherwise zero. Cancellation is checked
between bases, not during exponentiation. Independent calls share no mutable state.

`miller_bound(bound, n)` returns the inclusive cutoff for `n ≥ 2`.
Both integers must be initialized and distinct. [Mathematical basis](docs/mathematics.md).

```sh
./build/miller-check 65537
./build/miller-bench
```

The CLI exits 0 for completed classifications, 2 for malformed input, 3 for incomplete work.

## Security

Experimental; not audited or formally verified. Successful full scans explicitly
retain the GRH dependency. Not constant-time. GMP allocation failures are fatal;
a base counter reaching `UINT64_MAX` returns incomplete.

## Testing

Exhaustive sieve comparison below 20,000, strong pseudoprimes, known primes,
witnesses, cutoff rounding, cancellation and budget boundaries.

```sh
cmake -S . -B build-sanitize -DMILLER_SANITIZE=ON
cmake --build build-sanitize
ctest --test-dir build-sanitize --output-on-failure
```

## Speed

[Benchmarks](docs/benchmarks.md). Full Miller scans are substantially slower than
GMP’s probable-prime checks. The guarantees differ; no speed advantage is claimed.

## Contributing

Bug reports, mathematical review and focused patches are welcome.
[Open an issue](https://github.com/leonacostaok/libmiller/issues/new/choose) or read
[CONTRIBUTING.md](CONTRIBUTING.md). Private reports: [SECURITY.md](SECURITY.md).

## License

MIT. GMP is separately licensed.
