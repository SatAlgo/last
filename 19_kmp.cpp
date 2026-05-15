/* ============================================================
   TOPIC : KMP (Knuth-Morris-Pratt)
   PARADIGM : String Matching
   ============================================================

   CONCEPT
   -------
   On a mismatch, NAIVE algorithm restarts the pattern from
   index 0 and only shifts by 1. KMP precomputes a FAILURE
   ARRAY (LPS -- Longest Proper Prefix that is also Suffix)
   for the pattern, telling us how far we can shift the
   pattern WITHOUT re-comparing characters we already know match.
   Result: linear time, no matter what.

   LPS[i] = length of the longest proper prefix of pattern[0..i]
            which is also a suffix of the same substring.
   'Proper' means strictly shorter than the substring.

   The OUTPUT shows the LPS computation table for the pattern,
   plus a comparison count vs naive search.

   BRUTE FORCE: naive (file 18) -- O(n*m).

   COMPLEXITY
   ----------
       Best     : O(n + m)
       Average  : O(n + m)
       Worst    : O(n + m)        DETERMINISTIC linear
       Space    : O(m)             for LPS array

   REAL-WORLD USES
   ---------------
   * Text editors for large file search (less, gedit)
   * DNA sequence matching -- finding genes in genomes
   * Network packet inspection -- malware signature detection
   * Plagiarism detectors
   * Antivirus engines
   ============================================================ */

#include <iostream>
#include <vector>
#include <string>
using namespace std;

/* --- compute LPS array for pattern ------------------------- */
void computeLPS(string& pattern, vector<int>& lps) {
    int m = pattern.length();
    int len = 0;
    lps[0] = 0;
    int i = 1;
    while (i < m) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i++] = len;
        } else {
            if (len != 0) len = lps[len - 1];   // try shorter prefix
            else          lps[i++] = 0;
        }
    }
}

void KMPSearch(string text, string pattern) {
    int n = text.length();
    int m = pattern.length();
    vector<int> lps(m, 0);
    computeLPS(pattern, lps);

    /* === PRINT LPS COMPARISON TABLE ======================== */
    cout << "Pattern : " << pattern << "\n";
    cout << "Index   : ";
    for (int i = 0; i < m; i++) cout << i << " ";
    cout << "\n";
    cout << "Char    : ";
    for (int i = 0; i < m; i++) cout << pattern[i] << " ";
    cout << "\n";
    cout << "LPS[]   : ";
    for (int v : lps) cout << v << " ";
    cout << "\n\n";

    /* === SEARCH ============================================ */
    int i = 0, j = 0;
    int comparisons = 0;
    cout << "Matches at indices: ";
    while (i < n) {
        comparisons++;
        if (pattern[j] == text[i]) { i++; j++; }
        if (j == m) {
            cout << (i - j) << " ";
            j = lps[j - 1];     // continue searching using LPS
        }
        else if (i < n && pattern[j] != text[i]) {
            if (j != 0) j = lps[j - 1];   // shift smartly via LPS
            else        i++;
        }
    }
    cout << "\nKMP comparisons used: " << comparisons << "\n\n";
}

int main() {
    cout << "=== KMP run 1 ===\n";
    KMPSearch("ABABDABACDABABCABAB", "ABABCABAB");
    /* expected: match at 10 */

    cout << "=== KMP run 2 (worst case for naive) ===\n";
    KMPSearch("AAAAAAAAAAAAAAB", "AAAAB");
    /* expected: match at 10 */

    cout << "=== KMP run 3 (multiple matches) ===\n";
    KMPSearch("AABAACAADAABAABA", "AABA");
    /* expected: matches at 0 9 12 */
    return 0;
}

/* ============================================================
   LINE-BY-LINE EXPLANATION
   ------------------------
   computeLPS():
     'len' tracks the length of the longest prefix-suffix seen
     so far. At each position i we check whether the
     'len'-th character equals pattern[i]. If yes, we extend
     by one and set lps[i] = len, advance i. If no AND len > 0,
     fall back to a shorter prefix using the LPS value of
     position len-1. If no AND len == 0, lps[i] = 0, advance i.

   KMPSearch():
     Walk text with i and pattern with j. If they match, both
     advance. When j == m we've found a full match -- record
     i-j as match position, and set j = lps[j-1] to continue
     searching for more matches without losing context.
     On mismatch, if j>0 we use LPS to know how far to shift
     the pattern; if j==0 we advance i.

   ============================================================
   VIVA QUESTIONS
   --------------
   Q1. What does the LPS array represent?
   A.  For each prefix of the pattern, the length of the
       longest PROPER PREFIX that is also a SUFFIX. Example
       for 'ABAB' -> LPS = [0, 0, 1, 2]. The prefix 'ABAB' has
       'AB' as both a 2-char prefix and a 2-char suffix.

   Q2. Why is KMP LINEAR time?
   A.  The text pointer i never decreases. Each text character
       contributes at most O(1) operations: it's either matched
       (advance i), or used in a shift via LPS (j drops, i
       stays -- but j can drop at most m times TOTAL across
       all positions because it never goes below 0).

   Q3. How is LPS computed in O(m)?
   A.  Iteratively, using prior LPS values. If pattern[i] ==
       pattern[len], extend by 1 (constant work). Otherwise,
       jump to len = LPS[len-1] (a shorter prefix to try).
       Total work is amortized O(m).

   Q4. KMP vs Boyer-Moore -- when to use which?
   A.  KMP guarantees O(n+m) worst case for ANY input -- good
       for small alphabets or adversarial data. Boyer-Moore is
       faster in PRACTICE for large alphabets (English, DNA),
       sublinear AVERAGE, but worst case is similar.

   Q5. Can we MODIFY KMP for multiple patterns?
   A.  Yes -- AHO-CORASICK is a generalization. It builds a
       trie of all patterns and adds failure links similar to
       LPS. Linear in text length plus total pattern length.

   ============================================================
   CROSS QUESTIONS
   ---------------
   CQ1. Why is the prefix function called 'failure function'?
        Because on a mismatch (a 'failure'), it tells where to
        resume comparing. It encodes 'if we fail at position
        j, jump back to lps[j-1] and try matching there.'

   CQ2. Could we precompute LPS once and search MANY texts?
        Yes -- LPS is pattern-only. If the pattern is fixed and
        you search many documents, compute LPS once and reuse.

   CQ3. What's the WORST CASE for LPS computation?
        Patterns like 'AAAA...A' have LPS = [0,1,2,3,...,m-1]
        -- maximum redundancy. Computation still O(m) due to
        amortization. KMP-search on such patterns still O(n+m).

   CQ4. KMP vs Z-ALGORITHM?
        Z-algorithm computes Z[i] = length of longest substring
        starting at i that matches a prefix. It also gives
        linear-time string matching. Functionally equivalent to
        KMP; choice is taste.

   CQ5. Why doesn't the inner mismatch loop infinite-loop?
        Because j = lps[j-1] STRICTLY DECREASES whenever it's
        applied (lps values are < their index). Eventually j
        reaches 0, then we advance i. So progress is guaranteed.
   ============================================================ */
