#ifndef LIBMILLER_H
#define LIBMILLER_H
#include <gmp.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
/* PRIME_GRH records the mathematical dependency; it is not a probability. */
typedef enum { MILLER_NOT_PRIME=0, MILLER_PRIME=1, MILLER_PRIME_GRH=2,
               MILLER_INCOMPLETE=3, MILLER_INVALID=4 } miller_status;
typedef int (*miller_cancel_fn)(void *context);
typedef struct {
    uint64_t max_bases; /* zero means no caller-imposed limit */
    miller_cancel_fn cancelled; /* optional; checked before each base */
    void *context;
} miller_options;
typedef struct { miller_status status; uint64_t bases_tested; } miller_result;
/* n and bound must be initialized and distinct. n must be >= 2.
 * bound = min(n-2, ceil(49*bit_length(n)^2/50)). */
void miller_bound(mpz_t bound, const mpz_t n);
/* witness may be NULL; otherwise initialized, distinct from n. Set to zero
 * unless a divisor or strong Miller witness proves compositeness.
 * All mpz arguments must be valid. No globals; independent calls are safe.
 * Negative integers, 0 and 1 return NOT_PRIME, without a witness.
 * Cancellation/budget applies to base tests, not preprocessing or GMP powm. */
miller_result miller_check(const mpz_t n, const miller_options *options,
                           mpz_ptr witness);
const char *miller_status_name(miller_status status);
#ifdef __cplusplus
}
#endif
#endif
