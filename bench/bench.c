#include "miller.h"
#include <stdio.h>
#include <time.h>
int main(void) {
    const char *values[]={"2147483647","2305843009213693951",
      "170141183460469231731687303715884105727","341550071728321"};
    mpz_t n; mpz_init(n);
    printf("# GMP %s; CPU seconds; 5 repetitions; GMP reps=25 (different guarantee)\n",gmp_version);
    puts("input,bits,miller_status,bases,miller_seconds,gmp_result,gmp_seconds");
    for(size_t i=0;i<sizeof values/sizeof *values;++i) {
        mpz_set_str(n,values[i],10);
        clock_t start=clock(); miller_result r={MILLER_INVALID,0};
        for(int j=0;j<5;++j) r=miller_check(n,NULL,NULL);
        double a=(double)(clock()-start)/CLOCKS_PER_SEC/5;
        start=clock(); int probable=0;
        for(int j=0;j<5;++j) probable=mpz_probab_prime_p(n,25);
        double b=(double)(clock()-start)/CLOCKS_PER_SEC/5;
        printf("%s,%zu,%s,%llu,%.9f,%d,%.9f\n",values[i],mpz_sizeinbase(n,2),
          miller_status_name(r.status),(unsigned long long)r.bases_tested,a,probable,b);
    }
    mpz_clear(n); return 0;
}
