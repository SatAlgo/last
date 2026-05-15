/* ============================================================
   TOPIC : STRASSEN'S MATRIX MULTIPLICATION
   PARADIGM : Divide & Conquer
   ============================================================

   CONCEPT
   -------
   Multiply two n x n matrices in O(n^2.807) instead of O(n^3).
   Standard matrix multiplication of two 2x2 blocks needs 8
   multiplications; Strassen uses only 7 (with extra additions).
   Recurse to 1x1, then combine.

   Given A = | A11  A12 |    B = | B11  B12 |
            | A21  A22 |        | B21  B22 |

   Strassen's 7 products:
     M1 = (A11+A22) * (B11+B22)
     M2 = (A21+A22) * B11
     M3 = A11 * (B12-B22)
     M4 = A22 * (B21-B11)
     M5 = (A11+A12) * B22
     M6 = (A21-A11) * (B11+B12)
     M7 = (A12-A22) * (B21+B22)

   Then the result blocks:
     C11 = M1+M4-M5+M7
     C12 = M3+M5
     C21 = M2+M4
     C22 = M1-M2+M3+M6

   BRUTE FORCE
   -----------
   Naive matrix multiplication does n^3 scalar multiplications
   (3 nested loops). Strassen exploits 7-mult / 2x2 block
   identity recursively.

   RECURRENCE RELATION
   -------------------
       T(n) = 7 T(n/2) + O(n^2)
   Master Theorem: a = 7, b = 2, f(n) = n^2.
       log_b a = log_2 7 ~= 2.807
       f(n) = n^2, and 2 < 2.807  =>  CASE 1
       =>  T(n) = O(n^2.807)

   COMPLEXITY
   ----------
       Best     : O(n^2.807)
       Average  : O(n^2.807)
       Worst    : O(n^2.807)
       Space    : O(n^2)     for intermediate matrices
       Naive    : O(n^3)

   REAL-WORLD USES
   ---------------
   * Numerical libraries (BLAS / LAPACK) use it above n ~ 128
   * Deep learning -- matrix multiply is the core operation in
     fully-connected and convolution layers
   * Computer vision -- image transformations are matrix
     multiplies
   * Theoretical motivation -- sparked decades of 'fast matrix
     multiplication' research; current best ~O(n^2.371)
   ============================================================ */

#include <iostream>
#include <vector>
using namespace std;
typedef vector<vector<int>> Matrix;

Matrix add(Matrix A, Matrix B) {
    int n = A.size();
    Matrix C(n, vector<int>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] + B[i][j];
    return C;
}

Matrix sub(Matrix A, Matrix B) {
    int n = A.size();
    Matrix C(n, vector<int>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] - B[i][j];
    return C;
}

Matrix strassen(Matrix A, Matrix B) {
    int n = A.size();
    Matrix C(n, vector<int>(n));
    if (n == 1) { C[0][0] = A[0][0] * B[0][0]; return C; }

    int k = n / 2;
    Matrix A11(k, vector<int>(k)), A12(k, vector<int>(k)),
           A21(k, vector<int>(k)), A22(k, vector<int>(k));
    Matrix B11(k, vector<int>(k)), B12(k, vector<int>(k)),
           B21(k, vector<int>(k)), B22(k, vector<int>(k));

    /* split into 4 quadrants */
    for (int i = 0; i < k; i++)
        for (int j = 0; j < k; j++) {
            A11[i][j] = A[i][j];       A12[i][j] = A[i][j+k];
            A21[i][j] = A[i+k][j];     A22[i][j] = A[i+k][j+k];
            B11[i][j] = B[i][j];       B12[i][j] = B[i][j+k];
            B21[i][j] = B[i+k][j];     B22[i][j] = B[i+k][j+k];
        }

    /* SEVEN recursive multiplications (instead of 8) */
    Matrix M1 = strassen(add(A11, A22), add(B11, B22));
    Matrix M2 = strassen(add(A21, A22), B11);
    Matrix M3 = strassen(A11, sub(B12, B22));
    Matrix M4 = strassen(A22, sub(B21, B11));
    Matrix M5 = strassen(add(A11, A12), B22);
    Matrix M6 = strassen(sub(A21, A11), add(B11, B12));
    Matrix M7 = strassen(sub(A12, A22), add(B21, B22));

    /* combine into output quadrants */
    Matrix C11 = add(sub(add(M1, M4), M5), M7);
    Matrix C12 = add(M3, M5);
    Matrix C21 = add(M2, M4);
    Matrix C22 = add(sub(add(M1, M3), M2), M6);

    for (int i = 0; i < k; i++)
        for (int j = 0; j < k; j++) {
            C[i][j]     = C11[i][j];   C[i][j+k]   = C12[i][j];
            C[i+k][j]   = C21[i][j];   C[i+k][j+k] = C22[i][j];
        }
    return C;
}

int main() {
    Matrix A = {{1, 2}, {3, 4}};
    Matrix B = {{5, 6}, {7, 8}};

    cout << "A * B (Strassen) =\n";
    Matrix C = strassen(A, B);
    for (auto& row : C) {
        for (int x : row) cout << x << " ";
        cout << "\n";
    }
    /* Expected: 19 22 / 43 50 */

    // larger 4x4
    Matrix P = {{1, 2, 3, 4},
                {5, 6, 7, 8},
                {9, 10, 11, 12},
                {13, 14, 15, 16}};
    Matrix Q = {{1, 0, 0, 0},
                {0, 1, 0, 0},
                {0, 0, 1, 0},
                {0, 0, 0, 1}};      // identity
    cout << "\nP * I should equal P:\n";
    Matrix R = strassen(P, Q);
    for (auto& row : R) {
        for (int x : row) cout << x << " ";
        cout << "\n";
    }
    return 0;
}

/* ============================================================
   LINE-BY-LINE EXPLANATION
   ------------------------
   add() / sub(): element-wise matrix addition / subtraction
   on n x n matrices in O(n^2).

   strassen(): base case is n == 1 -- a 1x1 matrix is one
   scalar multiply. For n > 1:
     1. Compute k = n/2.
     2. Split A and B into four k x k quadrants each.
     3. Build the SEVEN auxiliary products M1..M7 using sums
        and differences of quadrants.
     4. Combine M1..M7 into output quadrants C11, C12, C21, C22.
     5. Stitch quadrants back into a single n x n result.

   The algebra is verified by expanding each Cij in terms of
   Aij and Bij -- you can check by hand that C11 = M1+M4-M5+M7
   equals A11*B11 + A12*B21.

   ============================================================
   VIVA QUESTIONS
   --------------
   Q1. What's the recurrence?
   A.  T(n) = 7T(n/2) + O(n^2). Master Theorem case 1:
       log_2(7) ~= 2.807, and f(n) = n^2 with 2 < 2.807,
       giving O(n^2.807).

   Q2. What are Strassen's main drawbacks?
   A.  (1) Only works on square matrices that are powers of 2
       (must pad otherwise).  (2) HIGH constant factors -- only
       beats naive past n ~= 100.  (3) Numerically less stable
       -- many adds/subs amplify floating-point error.

   Q3. What if n isn't a power of 2?
   A.  Pad with zero rows/cols up to next power of 2, run
       Strassen, strip padding. Or hybrid: Strassen on divisible
       block, naive on remainder.

   Q4. Why only 7 multiplications? Could we do 6?
   A.  Strassen's specific construction uses 7. It's proven the
       lower bound for 2x2 multiplication is at least 7 under
       reasonable models. Newer (more theoretical) algorithms
       achieve better asymptotic exponents via different ideas
       -- Coppersmith-Winograd (2.376), Le Gall (2.373).

   Q5. Is Strassen used in REAL libraries?
   A.  Yes, but only above a threshold (typically n > 64-128).
       BLAS libraries combine Strassen for large n with highly-
       tuned naive multiplication (using SIMD and cache
       blocking) for small blocks.

   ============================================================
   CROSS QUESTIONS
   ---------------
   CQ1. Why is Strassen numerically less stable?
        Each M-product involves several A or B entries summed
        with signs. With floating-point, those sums accumulate
        rounding error -- worse than naive's straightforward
        dot-products. For integer arithmetic this is moot.

   CQ2. How does the constant factor compare?
        Strassen does 18 additions/subtractions per 2x2 block
        versus naive's 4 -- huge overhead. Pays off only when
        the recursion depth amortizes the additions.

   CQ3. What's the best known matrix-multiply exponent today?
        ~2.371552 (Duan, Wu, Zhou 2023). Theoretical interest
        only -- those algorithms have galactic constants.

   CQ4. Could you parallelize Strassen?
        Yes -- the 7 M-products are INDEPENDENT, so 7 threads
        can compute them in parallel. Speedup roughly 7x on
        ideal hardware. Real implementations use this for GPUs.
   ============================================================ */
