#include "miller.h"
#include <stdio.h>
#include <stdlib.h>
#define CHECK(x) do { if (!(x)) { fprintf(stderr,"failed line %d: %s\n",__LINE__,#x); exit(1); } } while(0)
static int cancel(void *ctx) { unsigned *left=ctx; return (*left)-- == 0; }
static int prime(miller_status s) { return s==MILLER_PRIME || s==MILLER_PRIME_GRH; }
int main(void) {
    enum { N=20000 };
    unsigned char composite[N]={0};
    mpz_t n,w,b,t;
    mpz_inits(n,w,b,t,NULL);
    composite[0]=composite[1]=1;
    for(unsigned i=2;i*i<N;++i) if(!composite[i])
        for(unsigned j=i*i;j<N;j+=i) composite[j]=1;
    for(unsigned i=0;i<N;++i) {
        mpz_set_ui(n,i);
        miller_result r=miller_check(n,NULL,w);
        CHECK(prime(r.status)==!composite[i]);
        if(i>=2) {
            miller_bound(b,n);
            unsigned long bits=0,v=i;
            while(v) { ++bits; v>>=1; }
            unsigned long expected=(49*bits*bits+49)/50;
            if(expected>i-2) expected=i-2;
            CHECK(mpz_cmp_ui(b,expected)==0);
        }
    }
    const char *bad[]={"561","1105","1729","2047","1373653","25326001",
        "3215031751","341550071728321","3825123056546413051",
        "318665857834031151167461","3317044064679887385961981"};
    for(size_t i=0;i<sizeof bad/sizeof *bad;++i) {
        CHECK(mpz_set_str(n,bad[i],10)==0);
        CHECK(miller_check(n,NULL,w).status==MILLER_NOT_PRIME);
        CHECK(mpz_cmp_ui(w,1)>0 && mpz_cmp(w,n)<0);
        /* Independently verify returned witness using the defining powers. */
        if(!mpz_divisible_p(n,w)) {
            mpz_sub_ui(t,n,1);
            mp_bitcnt_t s=mpz_scan1(t,0);
            mpz_fdiv_q_2exp(t,t,s);
            mpz_powm(b,w,t,n);
            CHECK(mpz_cmp_ui(b,1)!=0);
            mpz_sub_ui(t,n,1);
            for(mp_bitcnt_t j=0;j<s;++j) {
                CHECK(mpz_cmp(b,t)!=0);
                mpz_mul(b,b,b); mpz_mod(b,b,n);
            }
        }
    }
    const char *good[]={"41","97","65537","2147483647",
        "2305843009213693951","170141183460469231731687303715884105727"};
    for(size_t i=0;i<sizeof good/sizeof *good;++i) {
        mpz_set_str(n,good[i],10);
        CHECK(miller_check(n,NULL,w).status==MILLER_PRIME_GRH);
        CHECK(mpz_sgn(w)==0);
    }
    mpz_set_ui(n,101); miller_bound(b,n);
    uint64_t count=(uint64_t)mpz_get_ui(b)-1;
    miller_options o={count-1,NULL,NULL};
    miller_result r=miller_check(n,&o,w);
    CHECK(r.status==MILLER_INCOMPLETE && r.bases_tested==count-1 && mpz_sgn(w)==0);
    o.max_bases=count;
    CHECK(miller_check(n,&o,NULL).status==MILLER_PRIME_GRH);
    unsigned left=0; o.max_bases=0; o.cancelled=cancel; o.context=&left;
    r=miller_check(n,&o,w); CHECK(r.status==MILLER_INCOMPLETE && r.bases_tested==0);
    left=2; r=miller_check(n,&o,w); CHECK(r.status==MILLER_INCOMPLETE && r.bases_tested==2);
    mpz_set_si(n,-17); CHECK(miller_check(n,NULL,NULL).status==MILLER_NOT_PRIME);
    /* 41 squared bypasses the small-divisor filter. */
    mpz_set_ui(n,1681); CHECK(miller_check(n,NULL,w).status==MILLER_NOT_PRIME);
    /* Conservative bound still works at a much larger bit length. */
    mpz_set_ui(n,1); mpz_mul_2exp(n,n,4095); mpz_add_ui(n,n,1);
    miller_bound(b,n); CHECK(mpz_cmp_ui(b,16441672)==0);
    mpz_clears(n,w,b,t,NULL);
    puts("passed: exhaustive sieve 0..19999, pseudoprimes, known primes, bounds, witnesses, budgets, cancellation");
    return 0;
}
