#include <stdio.h>

long pthFactor(long n, long p) {
    /*STRATEGY: Factors of n come in pairs (i, n/i) where i<= sqrt(n)
    We collecr only the "small" half, iterating up to sqrt(n)*/

    long small[100000]; // store factors <= sqrt(n); max divisor count for n<=10^15
    int k = 0;

    //Step 1: collect every factor up to sqrt(n) - paired factors handle the rest
    for (long i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            small[k++] = i; // small[0]=1, small[1]=first prime factor, etc.
        }

        /*Step 2:calculate total factors
        Mormally it's 2*k (each small factor i pairs with n/i)*/
        int is_perfect_square = (small[k - 1] * small[k - 1] == n);
        int total = 2 * k - is_perfect_square; // if n is a perfect square, we counted sqrt(n) twice

        //Step 3: If p exceeds total factor count, return 0 per the spec
        if (p > total) return 0;
    /*Step 4: decide which half p land in.*/
        if (p <= k)
            return small[p - 1];
        //Mirror index
        return n / small[total - p];
    }

    int main() {
        long n, p;

        printf("Enter n and p: ");
        scanf("%ld %ld", &n, &p);

        long result = pthFactor(n, p);
        printf("The %ld-th smallest factor of %ld is %ld\n", p, n, result);

        return 0;
    }