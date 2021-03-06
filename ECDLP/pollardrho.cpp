#include <stdio.h>
#include <stdlib.h>
#include "pollardrho.h"
#include "random.h"

/* Global state to generate random numbers */
//gmp_randstate_t state;

BigInt calculate_result(const BigInt c1, 
                        const BigInt c2,
                        const BigInt d1, 
                        const BigInt d2, 
                        const BigInt order)
{
    BigInt numerator, denominator, result;
    if(d1 == d2) {
        printf("Cannot calculate the discrete log\n");
        return -1;
    }

    /* numerator = (c1 - c2) mod order */
    numerator = (c1 - c2) % order;
    if(numerator < 0) numerator += order;

    /* denominator (d2 - d1)^(-1) mod order */
    denominator = (d2 - d1) % order;
    if(denominator < 0) denominator += order;
    denominator = modInverse(denominator, order);

    /* result */
    result = (numerator * denominator) % order;

    return result;
}


/* this function returns P.x % L */
/* Pay attention to the point at infinity, which is represented by (-1, -1) */
unsigned long partition_function(const Point P) {
    unsigned long result;
    if(P.x == -1)
        result = L - 1;
    else 
#ifdef _LIB_GMP
        result = P.x.get_ui();
#else
        P.x.ToUInt(result);
#endif

    return result % L;
}


int init_branches(Triple *branches,
                  const EllipticCurve ec,
                  const Point P, 
                  const Point Q)
{
    BigInt aj, bj;
    int i;

    for(i = 0; i < L; i++) {
        Point Rj, Ptemp, Qtemp;

        aj = random_number(ec.order);
        bj = random_number(ec.order);
        ecc_mul(Ptemp, ec, aj, P);
        ecc_mul(Qtemp, ec, bj, Q);
        ecc_add(Rj, ec, Ptemp, Qtemp);
        branches[i].c = aj;
        branches[i].d = bj;
        branches[i].point = Rj;
    }

    return 0;
}

void r_adding_walk(const EllipticCurve ec,
                   BigInt& c,
                   BigInt& d, 
                   Point& X, 
                   const Triple* branches, 
                   const unsigned long j)
{
    c = (c + branches[j].c) % ec.order;
    d = (d + branches[j].d) % ec.order;
    ecc_add(X, ec, X, branches[j].point);
}

bool isDistinguished(Point P) {
    return P.x != -1 && count_1bits(P.x) <= 5;
}


/*
 * See Hacker's Delight chapter 5
 * This function will work to a 64bits number.
 * For numbers greater than 64bits it should be
 * modified
 */
BigInt count_1bits(BigInt x)
{
    x = x - ((x >> 1) & 0x5555555555555555);
    x = (x & 0x3333333333333333) + ((x >> 2) & 0x3333333333333333);
    x = (x + (x >> 4)) & 0x0f0f0f0f0f0f0f0f;
    x = x + (x >> 8);
    x = x + (x >> 16);

    return x & 0x000000000000003F;
}
