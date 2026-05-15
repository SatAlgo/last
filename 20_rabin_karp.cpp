/* ============================================================
   TOPIC : RABIN-KARP STRING MATCHING
   PARADIGM : String Matching (Hashing)
   ============================================================

   CONCEPT
   -------
   Instead of comparing characters, compare HASHES. For each
   m-length window of the text, compute its hash and compare
   with the pattern's hash. Hashes match -> verify char by
   char (to rule out hash collisions). Use a ROLLING HASH so
   each new window costs O(1) instead of O(m).

   Rolling hash transition (going from window i to i+1):
       new_hash = ((old_hash - text[i] * h) * d + text[i+m]) mod q
   where d = alphabet size (256 for ASCII)
         h = d^(m-1) mod q  (precomputed)
         q = a prime modulus

   Key benefit: USEFUL FOR MULTI-PATTERN SEARCH. With a set of
   pattern hashes, you can search text in O(n + k*m) for k
   patterns (vs O(n*k) for separate searches).

   BRUTE FORCE: naive O(n*m).

   COMPLEXITY
   ----------
       Best     : O(n + m)
       Average  : O(n + m)
       Worst    : O(n * m)        many spurious hash matches
       Space    : O(1)

   REAL-WORLD USES
   ---------------
   * Plagiarism detection -- hash many candidate phrases at
     once, sweep through document
   * Detecting duplicate files (rsync's rolling-checksum)
   * Spam / fingerprint detection
   * BIOINFORMATICS -- finding multiple k-mers in DNA
   * Substring deduplication for storage systems
   ============================================================ */

#include <iostream>
#include <string>
using namespace std;

#define d 256            // alphabet size (ASCII)
const int q = 101;       // a small prime modulus (use a larger prime in production)

void rabinKarp(string text, string pattern) {
    int n = text.length();
    int m = pattern.length();
    if (m == 0 || n < m) { cout << "No match possible.\n"; return; }

    int p = 0;   // hash of pattern
    int t = 0;   // hash of current text window
    int h = 1;   // d^(m-1) mod q
    for (int i = 0; i < m - 1; i++) h = (h * d) % q;

    /* initial hashes for pattern and first window */
    for (int i = 0; i < m; i++) {
        p = (d * p + pattern[i]) % q;
        t = (d * t + text[i])    % q;
    }

    int collisions = 0, matches = 0;
    cout << "Text    : " << text << "\n";
    cout << "Pattern : " << pattern << "  (hash=" << p << ")\n";
    cout << "Matches at indices: ";

    for (int i = 0; i <= n - m; i++) {
        if (p == t) {
            /* hashes match -- verify char by char (handle collisions) */
            int j;
            for (j = 0; j < m; j++)
                if (text[i + j] != pattern[j]) break;
            if (j == m) { cout << i << " "; matches++; }
            else        collisions++;
        }
        /* roll hash forward */
        if (i < n - m) {
            t = (d * (t - text[i] * h) + text[i + m]) % q;
            if (t < 0) t += q;
        }
    }
    cout << "\nMatches : " << matches
         << "   Hash collisions (false positives): " << collisions << "\n\n";
}

int main() {
    rabinKarp("GEEKS FOR GEEKS", "GEEK");
    /* expected matches at 0 and 10 */

    rabinKarp("AABAACAADAABAABA", "AABA");
    /* expected matches at 0, 9, 12 */

    rabinKarp("ABABABABABABABAB", "ABAB");
    /* many overlapping matches */

    /* worst-case-like (high collision rate with small prime) */
    rabinKarp("AAAAAAAAAAAAAAB", "AAAAB");
    return 0;
}

/* ============================================================
   LINE-BY-LINE EXPLANATION
   ------------------------
   d, q: alphabet size and prime. Larger q -> fewer collisions
   but bigger intermediate values.

   h = d^(m-1) mod q: precomputed once, used in rolling update.

   Initial hashes: standard polynomial hash p(x) = sum c[i]*d^(m-1-i).

   Main loop:
     - if pattern hash == window hash, do char-by-char verify
       to discard COLLISIONS (different strings with same hash).
     - Roll hash forward: subtract contribution of leaving char
       (text[i] * h), multiply by d (shift left), add new
       character. Mod q at the end.

   The 'if (t < 0) t += q' handles negative results from
   subtraction in modular arithmetic.

   ============================================================
   VIVA QUESTIONS
   --------------
   Q1. Why is Rabin-Karp's WORST CASE still O(n*m)?
   A.  If every window happens to hash-match the pattern (a
       weak / colliding hash function), we verify all m chars
       at every position. With a good random hash, this is
       extremely unlikely.

   Q2. What is a SPURIOUS HIT / hash collision?
   A.  Two different strings have the same hash. RK must verify
       on a hash match -- otherwise it would report false
       matches. The verify step costs O(m).

   Q3. Why is q chosen as a PRIME?
   A.  Modular arithmetic with a prime modulus spreads hash
       values more uniformly across [0, q-1], reducing
       collisions. Composite moduli have 'patterns' an
       adversary can exploit.

   Q4. Why is Rabin-Karp PREFERRED for multi-pattern search?
   A.  You can precompute hashes for k patterns (one hash each),
       store them in a hash set, and a single sweep over the
       text checks all k patterns simultaneously. O(n + k*m)
       instead of O(n*k).

   Q5. What's the role of ROLLING HASH?
   A.  Recomputing a hash from scratch for each m-length window
       is O(m) per window -> O(n*m) total. Rolling hash updates
       in O(1) by removing the leaving character and adding the
       entering one.

   ============================================================
   CROSS QUESTIONS
   ---------------
   CQ1. Why is RK considered RANDOMIZED in some formulations?
        Choosing q at random (from a large set of primes) makes
        the algorithm a MONTE CARLO algorithm: very likely
        O(n+m), but unlucky inputs are O(n*m). Verifying chars
        guarantees CORRECTNESS regardless.

   CQ2. Could we skip the char-by-char verification?
        That would be a MONTE CARLO version -- fast but
        sometimes WRONG (reports false matches on collisions).
        Used for fingerprint sketching where false positives
        are acceptable.

   CQ3. RK vs KMP -- which is better in practice for SINGLE
        pattern search?
        KMP -- it's deterministic O(n+m). RK's overhead (modular
        arithmetic) makes it slower for single-pattern. RK's
        advantage is multi-pattern.

   CQ4. Why is q usually a LARGE prime in real implementations?
        With q ~ 10^9 (fits in int64), collision probability
        drops to ~1/q per window. Then 'verify on match' is
        rarely needed. Small q (like 101 in demos) shows the
        idea but has many collisions.

   CQ5. How does this generalize to 2D pattern matching?
        Bird-Baeza-Yates algorithm: hash each row of the
        pattern, then hash columns of those row-hashes. Allows
        2D rolling-hash pattern matching in images.
   ============================================================ */
