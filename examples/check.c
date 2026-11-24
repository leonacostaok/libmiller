#include "miller.h"
#include <stdio.h>
int main(int argc, char **argv) {
    mpz_t n, witness;
    if (argc != 2) { fprintf(stderr,"usage: miller-check INTEGER\n"); return 2; }
    mpz_inits(n,witness,NULL);
    if (mpz_set_str(n,argv[1],10)) {
        fprintf(stderr,"invalid decimal integer\n");
        mpz_clears(n,witness,NULL); return 2;
    }
    miller_result r=miller_check(n,NULL,witness);
    gmp_printf("%s bases=%llu witness=%Zd\n",miller_status_name(r.status),
               (unsigned long long)r.bases_tested,witness);
    mpz_clears(n,witness,NULL);
    return r.status==MILLER_INCOMPLETE ? 3 : 0;
}
