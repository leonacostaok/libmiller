export type Status = 'not-prime' | 'prime' | 'prime-grh' | 'incomplete';
export interface Options {
  /** Zero means unlimited. Must fit uint64. */
  maxBases?: bigint;
  /** Checked between bases. Exceptions propagate to the caller. */
  cancelled?: () => boolean;
}
export interface Result {
  status: Status;
  basesTested: bigint;
  /** Zero unless a divisor or failing base is found. */
  witness: bigint;
}
/** Inclusive cutoff; n must be at least 2. */
export declare function millerBound(n: bigint): bigint;
/** Synchronous; full prime results depend on Dirichlet GRH. */
export declare function checkPrime(n: bigint, options?: Options): Result;
