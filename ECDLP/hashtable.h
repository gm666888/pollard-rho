#ifndef _HASHTABLE
#define _HASHTABLE

#include "point.h"
#define hashkey_t unsigned long


struct Triple {
    BigInt c;
    BigInt d;
    Point point;

    Triple() {}
    ~Triple() {}
    Triple(BigInt c, BigInt d, Point P) : c(c), d(d), point(P) {}
};

struct Chain;

struct Hashtable {
    long size;
    long n_elems;
    long maxnodes;
    Chain* chain;

    Hashtable (long size);
    /*
     * If insert return false, then it found a collision.
     * The c_triple reference will store the value of the triple
     * which have a collision.
     */
    bool insert(const Triple triple,
                Triple& c_triple);
};

#endif
