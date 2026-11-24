const SMALL_PRIMES = [2n, 3n, 5n, 7n, 11n, 13n, 17n, 19n, 23n, 29n, 31n, 37n];
const MAX_BASES = (1n << 64n) - 1n;

function integer(n) {
  if (typeof n !== 'bigint') throw new TypeError('n must be a bigint');
}

/** Inclusive conservative Miller/Bach cutoff, computed without floating point. */
export function millerBound(n) {
  integer(n);
  if (n < 2n) throw new RangeError('n must be at least 2');
  const bits = BigInt(n.toString(2).length);
  const bound = (49n * bits * bits + 49n) / 50n;
  return bound < n - 2n ? bound : n - 2n;
}

function powmod(base, exponent, modulus) {
  let result = 1n;
  while (exponent > 0n) {
    if (exponent & 1n) result = (result * base) % modulus;
    exponent >>= 1n;
    if (exponent > 0n) base = (base * base) % modulus;
  }
  return result;
}

/** Synchronous Miller scan. Use a worker for large inputs in a browser. */
export function checkPrime(n, options = {}) {
  integer(n);
  if (options === null || typeof options !== 'object' || Array.isArray(options))
    throw new TypeError('options must be an object');
  const { maxBases = 0n, cancelled } = options;
  if (typeof maxBases !== 'bigint' || maxBases < 0n || maxBases > MAX_BASES)
    throw new RangeError('maxBases must be a bigint in [0, 2**64 - 1]');
  if (cancelled !== undefined && typeof cancelled !== 'function')
    throw new TypeError('cancelled must be a function');
  let basesTested = 0n;
  const result = (status, witness = 0n) => ({ status, basesTested, witness });
  if (n < 2n) return result('not-prime');
  for (const p of SMALL_PRIMES) {
    if (n === p) return result('prime');
    if (n % p === 0n) return result('not-prime', p);
  }
  const nm1 = n - 1n;
  let d = nm1;
  let s = 0;
  while ((d & 1n) === 0n) { d >>= 1n; s++; }
  const bound = millerBound(n);
  for (let a = 2n; a <= bound; a++) {
    if (basesTested === MAX_BASES || (maxBases !== 0n && basesTested >= maxBases) || cancelled?.())
      return result('incomplete');
    let x = powmod(a, d, n);
    basesTested++;
    if (x === 1n || x === nm1) continue;
    let passes = false;
    for (let j = 1; j < s; j++) {
      x = (x * x) % n;
      if (x === nm1) { passes = true; break; }
      if (x === 1n) break;
    }
    if (!passes) return result('not-prime', a);
  }
  return result('prime-grh');
}
