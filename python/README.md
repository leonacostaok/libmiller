# libmiller · Python

Typed Python bindings to the C/GMP library. Python ≥ 3.9; no Python runtime dependencies.

## Usage

From the repository root, build the shared C library, then:

```sh
python -m pip install ./python
export LIBMILLER_PATH="$PWD/build/libmiller.so" # macOS: libmiller.dylib
```

```python
from libmiller import check_prime, Status

result = check_prime(65537, max_bases=10000)
assert result.status == Status.PRIME_GRH
print(result.bases_tested, result.witness)
```

`n` must be an `int`; booleans and floats are rejected. `max_bases` must fit uint64;
zero means unlimited. Results use the C API statuses. Inspect `status` explicitly.
Full-scan primality depends on Dirichlet GRH. The wheel does not bundle GMP or libmiller.

Pass `library="/absolute/path/to/libmiller.so"` to override library discovery.
Python exposes base limits; cancellation callbacks are available through C.

## Tests

```sh
PYTHONPATH=python python -m unittest discover -s python/tests
PYTHONPATH=python python python/tests/cross_language.py
```

Set `LIBMILLER_PATH` first. Cross-language tests also require Node.js.

MIT licensed. Experimental; not audited or formally verified.
