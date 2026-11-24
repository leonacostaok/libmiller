#include "miller.h"
#include <stddef.h>

void miller_bound(mpz_t bound, const mpz_t n) {
    size_t bits = mpz_sizeinbase(n, 2);
    mpz_t cap;
    mpz_init(cap);
    /* Import size_t without narrowing to unsigned long. */
    mpz_import(bound, 1, 1, sizeof bits, 0, 0, &bits);
    mpz_mul(bound, bound, bound);
    mpz_mul_ui(bound, bound, 49);
    mpz_cdiv_q_ui(bound, bound, 50);
    mpz_sub_ui(cap, n, 2);
    if (mpz_cmp(bound, cap) > 0) mpz_set(bound, cap);
    mpz_clear(cap);
}

miller_result miller_check(const mpz_t n, const miller_options *o,
                           mpz_ptr witness) {
    static const unsigned long small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    miller_result result = {MILLER_NOT_PRIME, 0};
    mpz_t d, nm1, a, x, bound;
    mp_bitcnt_t s;
    if (witness) mpz_set_ui(witness, 0);
    if (mpz_cmp_ui(n, 2) < 0) return result;
    for (size_t i=0; i<sizeof small/sizeof *small; ++i) {
        if (mpz_cmp_ui(n, small[i]) == 0) {
            result.status = MILLER_PRIME;
            return result;
        }
        if (mpz_divisible_ui_p(n, small[i])) {
            if (witness) mpz_set_ui(witness, small[i]);
            return result;
        }
    }
    mpz_inits(d, nm1, a, x, bound, NULL);
    mpz_sub_ui(nm1, n, 1);
    s = mpz_scan1(nm1, 0);
    mpz_fdiv_q_2exp(d, nm1, s);
    miller_bound(bound, n);
    result.status = MILLER_PRIME_GRH;
    for (mpz_set_ui(a, 2); mpz_cmp(a, bound) <= 0; mpz_add_ui(a,a,1)) {
        if (result.bases_tested == UINT64_MAX ||
            (o && ((o->max_bases && result.bases_tested >= o->max_bases) ||
                   (o->cancelled && o->cancelled(o->context))))) {
            result.status = MILLER_INCOMPLETE;
            break;
        }
        mpz_powm(x, a, d, n);
        ++result.bases_tested;
        if (mpz_cmp_ui(x,1)==0 || mpz_cmp(x,nm1)==0) continue;
        int passes = 0;
        for (mp_bitcnt_t j=1; j<s; ++j) {
            mpz_mul(x,x,x);
            mpz_mod(x,x,n);
            if (mpz_cmp(x,nm1)==0) { passes=1; break; }
            if (mpz_cmp_ui(x,1)==0) break;
        }
        if (!passes) {
            result.status = MILLER_NOT_PRIME;
            if (witness) mpz_set(witness,a);
            break;
        }
    }
    mpz_clears(d,nm1,a,x,bound,NULL);
    return result;
}
const char *miller_status_name(miller_status s) {
    switch(s) {
    case MILLER_NOT_PRIME: return "not-prime";
    case MILLER_PRIME: return "prime";
    case MILLER_PRIME_GRH: return "prime-grh";
    case MILLER_INCOMPLETE: return "incomplete";
    default: return "invalid";
    }
}
