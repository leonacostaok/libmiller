import { test } from 'node:test';
import assert from 'node:assert/strict';
import { readFileSync } from 'node:fs';
import { checkPrime, millerBound } from './index.js';

const vectors = JSON.parse(readFileSync(new URL('../tests/vectors.json', import.meta.url)));
test('shared vectors', () => {
  for (const v of vectors) {
    const r = checkPrime(BigInt(v.n), { maxBases: BigInt(v.max_bases) });
    assert.equal(r.status, v.status, v.n);
    if (r.status !== 'not-prime') assert.equal(r.witness, 0n);
  }
});
test('independent sieve', () => {
  const composite = new Uint8Array(2000);
  composite[0] = composite[1] = 1;
  for (let i = 2; i * i < composite.length; i++)
    if (!composite[i]) for (let j = i * i; j < composite.length; j += i) composite[j] = 1;
  for (let i = 0; i < composite.length; i++) {
    const r = checkPrime(BigInt(i));
    assert.equal(['prime', 'prime-grh'].includes(r.status), !composite[i], String(i));
  }
});
test('bound and cancellation', () => {
  assert.equal(millerBound(101n), 49n);
  assert.equal(millerBound((1n << 4095n) + 1n), 16441672n);
  assert.deepEqual(checkPrime(101n, {cancelled: () => true}),
    {status: 'incomplete', basesTested: 0n, witness: 0n});
  let calls = 0;
  assert.equal(checkPrime(101n, {cancelled: () => calls++ >= 2}).basesTested, 2n);
  assert.throws(() => checkPrime(101n, {cancelled: () => {throw Error('cancel');}}), /cancel/);
  assert.equal(checkPrime(101n, {maxBases: (1n << 64n)-1n}).status, 'prime-grh');
});
test('strict input types', () => {
  for (const n of [17, '17', true, null, undefined]) assert.throws(() => checkPrime(n), TypeError);
  for (const maxBases of [1, -1n, 1n << 64n, null])
    assert.throws(() => checkPrime(101n, {maxBases}), RangeError);
  for (const options of [null, [], true]) assert.throws(() => checkPrime(101n, options), TypeError);
  assert.throws(() => millerBound(1n), RangeError);
  assert.throws(() => checkPrime(101n, {cancelled: true}), TypeError);
});
