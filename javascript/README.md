# libmiller · JavaScript / TypeScript

Deterministic Miller primality testing with native BigInt. No runtime dependencies.
ES modules for Node.js ≥ 18 and browsers supporting BigInt; TypeScript declarations included.

## Usage

```js
import { checkPrime, millerBound } from './index.js';

const result = checkPrime(65537n, { maxBases: 10000n });
// { status: 'prime-grh', basesTested: 283n, witness: 0n }
```

| Field | Meaning |
| --- | --- |
| `status` | `not-prime`, `prime`, `prime-grh` or `incomplete` |
| `basesTested` | Number of completed base tests, as bigint |
| `witness` | Divisor or failing Miller base; zero otherwise |

Inputs and budgets must be `bigint`. Zero budget means unlimited; budgets must fit uint64.
`millerBound(n)` returns the inclusive cutoff for `n ≥ 2n`.
`cancelled: () => boolean` is checked between bases; exceptions propagate.

Full-scan primality depends on Dirichlet GRH. Small primes use `prime` independently.
An interrupted scan returns `incomplete`, never a prime result.

Calls are synchronous. Use a worker for large inputs; a timer on the same thread
cannot interrupt a running scan. Native BigInt operations are not constant-time.

```sh
npm test
npm pack
```

Source package only; not yet published to npm. MIT licensed. Experimental; not audited.
