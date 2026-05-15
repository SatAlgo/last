/* ============================================================
   TOPIC : KARATSUBA MULTIPLICATION
   PARADIGM : Divide & Conquer
   ============================================================

   CONCEPT
   -------
   Multiply two n-digit numbers in O(n^1.585) instead of O(n^2).
   Standard 'school' multiplication makes 4 sub-multiplications
   per split; Karatsuba makes only 3 using an algebraic trick.

   Let x = a * 10^(n/2) + b
       y = c * 10^(n/2) + d
   Then xy = ac * 10^n  +  (ad + bc) * 10^(n/2)  +  bd
   Key insight:  ad + bc  =  (a+b)(c+d) - ac - bd
   So only THREE multiplications: ac, bd, (a+b)(c+d).
   The fourth term is obtained by SUBTRACTION (cheap).

   BRUTE FORCE
   -----------
   School-book multiplication on n-digit numbers takes
   O(n^2) digit-multiplications:
   For two 4-digit numbers, that's 16 products.
   Karatsuba reduces it to ~3 sub-products per recursion, so
   for n digits the count is n^(log_2 3) = n^1.585.

   RECURRENCE RELATION
   -------------------
       T(n) = 3 T(n/2) + O(n)
   Master Theorem: a = 3, b = 2, f(n) = n.
       log_b a = log_2 3 = 1.585...
       f(n) = n^1, and 1 < 1.585  =>  CASE 1
       =>  T(n) = O(n^log_2 3) = O(n^1.585)

   COMPLEXITY
   ----------
       Best     : O(n^1.585)
       Average  : O(n^1.585)
       Worst    : O(n^1.585)
       Space    : O(n)         recursion + intermediate storage
       Compared to school: O(n^2)

   REAL-WORLD USES
   ---------------
   * RSA / ECC cryptography (multiplying 1024-4096 bit numbers
     in every HTTPS handshake)
   * Python's int, Java BigInteger, GMP library
   * Computing pi to billions of digits in scientific computing
   * Number-theoretic algorithms (modular exponentiation,
     factorization)
   ============================================================ */

#include <iostream>
#include <string>
#include <cmath>
using namespace std;

long long karatsuba(long long x, long long y) {
    /* Base case: small numbers multiply directly. */
    if (x < 10 || y < 10) return x * y;

    /* n = max length in digits of x and y. */
    int n = max(to_string(x).length(), to_string(y).length());
    int half = n / 2;
    long long power = (long long)pow(10, half);

    /* Split each number into high (a, c) and low (b, d) halves. */
    long long a = x / power, b = x % power;
    long long c = y / power, d = y % power;

    /* THREE recursive multiplications. */
    long long ac = karatsuba(a, c);
    long long bd = karatsuba(b, d);
    long long ad_plus_bc = karatsuba(a + b, c + d) - ac - bd;

    return ac * (long long)pow(10, 2 * half)
         + ad_plus_bc * power
         + bd;
}

int main() {
    long long x = 1234, y = 5678;
    cout << x << " * " << y << " = "
         << karatsuba(x, y)
         << "  (verify: " << x * y << ")\n";

    long long a = 12345, b = 6789;
    cout << a << " * " << b << " = "
         << karatsuba(a, b)
         << "  (verify: " << a * b << ")\n";

    // edge cases
    cout << "0 * 99 = " << karatsuba(0, 99) << "\n";
    cout << "7 * 8  = " << karatsuba(7, 8) << "\n";   // base case
    return 0;
}

/* ============================================================
   LINE-BY-LINE EXPLANATION
   ------------------------
   Base case (x < 10 || y < 10):
     Direct multiplication ends recursion. (For real bignum
     libraries the threshold is around 30-100 digits where the
     constant overhead of recursion beats school multiplication.)

   Split:
     'half' = n/2 digits.  'power' = 10^half.
     a = x / 10^half  (upper half)
     b = x % 10^half  (lower half)
     same for c, d on y.

   Three multiplications:
     ac           : product of high halves.
     bd           : product of low  halves.
     (a+b)(c+d)   : combined sum products; we subtract ac and
                    bd to recover (ad + bc).

   Recombine:
     xy = ac * 10^(2*half) + (ad+bc) * 10^half + bd.

   ============================================================
   VIVA QUESTIONS
   --------------
   Q1. Why exactly 3 multiplications, not 4?
   A.  Identity: (a+b)(c+d) = ac + ad + bc + bd.
       So ad + bc = (a+b)(c+d) - ac - bd.
       We compute ac, bd, (a+b)(c+d) -- three products. The
       cross term ad+bc is obtained by subtraction, which is
       cheap (O(n) for n-digit numbers).

   Q2. What's the recurrence?
   A.  T(n) = 3T(n/2) + O(n). Master Theorem case 1 with
       log_2(3) ~= 1.585 gives O(n^1.585).

   Q3. Is Karatsuba always faster than school multiplication?
   A.  No -- for small n (under ~30 digits), the recursion
       overhead and extra additions make school multiplication
       faster. Real bignum libraries crossover above some
       threshold.

   Q4. Why do we add 'a+b' and 'c+d'? Can those overflow?
   A.  Yes for very large numbers. Real implementations either
       use arbitrary-precision arithmetic (so no overflow) or
       carefully track the carry digit.

   Q5. What's faster than Karatsuba?
   A.  Toom-Cook (Toom-3): O(n^1.465).  Schoenhage-Strassen
       (FFT-based): O(n log n log log n).  Harvey-Hoeven (2019):
       theoretical O(n log n).

   ============================================================
   CROSS QUESTIONS
   ---------------
   CQ1. Why does the recursion bottom out at single digits?
        Because single-digit multiplication is a constant-time
        machine instruction. Recursion exists only because
        large numbers don't fit one machine word.

   CQ2. Where exactly does the n^log_2(3) come from?
        At each level of recursion we have 3 subproblems each
        of half the size. After log_2(n) levels we reach 3^(log_2 n)
        = n^(log_2 3) base cases. Linear merging at each level
        adds O(n) per level. The geometric series dominates ->
        O(n^log_2 3).

   CQ3. Strassen and Karatsuba -- same idea?
        Same SPIRIT: both reduce constant from naive divide-
        and-conquer using algebraic tricks (Karatsuba: 4->3
        for multiplication; Strassen: 8->7 for matrix multiply).
        Both opened doors to extensive theoretical research.

   CQ4. How would Karatsuba be implemented on REAL bignum types
        like Python's int?
        Numbers stored as arrays of 'limbs' (each limb is a
        machine word, say 64 bits). Split, recurse, combine
        with shift-by-limb-count instead of multiply-by-power-
        of-10. CPython uses Karatsuba above ~70-digit threshold.
   ============================================================ */
