#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


uint64_t factorial (uint64_t n);


int main (void)
{
    uint64_t f, n;
    n = 5;
    f = factorial(n);
    printf("%" PRIu64 "! = %" PRIu64 "\n", n, f);
    
    return EXIT_SUCCESS;
}


uint64_t factorial (const uint64_t n)
{
    if (n <= 1)
    {
        return 1;
    }
    return n * factorial(n - 1);
}
