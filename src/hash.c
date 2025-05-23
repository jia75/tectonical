#include "hash.h"

static int abs(int a) {
    return a<0?-a:a;
}

/* Returns an integer from 0 to 255 */
int randomHash(int in) {
    int a = abs(in*100*in+24*in+47)%257;
    return a%256;
}

/* Hash from 0 to max */
int hashInRange(int max, int in) {
    int out = randomHash(in);
    ++in;
    while (out < max) {
        out += randomHash(in+2)*randomHash(in)*randomHash(in+1);
        in += 3;
    }
    return abs(out)%max;
}