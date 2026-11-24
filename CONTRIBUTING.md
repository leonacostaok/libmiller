# Contributing

Reports, test vectors, mathematical review and focused pull requests are welcome.

- Search existing issues before opening one. Include the input, expected result,
  actual result, version and a minimal reproducer.
- Add a regression test for fixes. Keep C, Python and JavaScript results consistent.
- Changes to bounds or skipped bases need a proof and primary reference.
- Include reproducible measurements for performance changes. Compare equivalent guarantees.
- Keep dependencies and API surface small. Discuss API changes in an issue first.

## Checks

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
npm --prefix javascript test
```

For Python and cross-language tests, see [python/README.md](python/README.md).
Contributions are licensed under MIT. See [SECURITY.md](SECURITY.md) for vulnerabilities.
