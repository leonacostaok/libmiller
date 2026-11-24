import { readFileSync } from 'node:fs';
import { checkPrime } from './index.js';
const cases = JSON.parse(readFileSync(0, 'utf8'));
console.log(JSON.stringify(cases.map(v => checkPrime(BigInt(v.n), {maxBases: BigInt(v.max_bases)})),
  (_, value) => typeof value === 'bigint' ? value.toString() : value));
