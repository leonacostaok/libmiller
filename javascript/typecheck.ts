import { checkPrime, millerBound, type Result } from './index.js';
const result: Result = checkPrime(65537n, { maxBases: 1000n, cancelled: () => false });
const count: bigint = result.basesTested;
const bound: bigint = millerBound(65537n);
void count; void bound;
// @ts-expect-error Numbers must not silently lose precision.
checkPrime(65537);
// @ts-expect-error Budgets must be bigint.
checkPrime(65537n, { maxBases: 1 });
