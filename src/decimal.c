#include "miller.h"
#include <string.h>

unsigned int miller_abi_version(void) { return 1; }

int miller_check_decimal(const char *decimal, uint64_t max_bases,
                         uint64_t *bases_tested, char *witness, size_t witness_size) {
    if (bases_tested) *bases_tested = 0;
    if (witness && witness_size) witness[0] = '\0';
    if (!decimal || (!witness && witness_size)) return MILLER_INVALID;
    size_t length = strlen(decimal);
    if (witness && witness_size <= length) return MILLER_INVALID;
    const char *digit = decimal;
    if (*digit == '-') ++digit;
    if (!*digit) return MILLER_INVALID;
    for (; *digit; ++digit)
        if (*digit < '0' || *digit > '9') return MILLER_INVALID;
    mpz_t n, factor;
    mpz_inits(n, factor, NULL);
    if (mpz_set_str(n, decimal, 10)) {
        mpz_clears(n, factor, NULL);
        return MILLER_INVALID;
    }
    miller_options options = {max_bases, NULL, NULL};
    miller_result result = miller_check(n, &options, factor);
    if (bases_tested) *bases_tested = result.bases_tested;
    if (witness) mpz_get_str(witness, 10, factor);
    mpz_clears(n, factor, NULL);
    return (int)result.status;
}
