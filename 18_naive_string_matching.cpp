/* ============================================================
   TOPIC : NAIVE STRING MATCHING
   PARADIGM : String Matching (brute force)
   ============================================================

   CONCEPT
   -------
   Slide the pattern across the text ONE position at a time.
   At each position, compare character by character. If all m
   characters match, report the position. NO preprocessing
   needed.

   Comparison shows that naive is the brute force baseline:
   later files (KMP, Rabin-Karp, Boyer-Moore) demonstrate the
   speedups possible with cleverer techniques.

   BRUTE FORCE: this IS the brute force method.

   COMPLEXITY
   ----------
       Best     : O(n)         pattern mismatches at 1st char of every shift
       Average  : O(n*m)        on random text -- but constant low
       Worst    : O(n*m)        e.g. text='aaa...a' pattern='aa..ab'
       Space    : O(1)         no preprocessing
   where n = text length, m = pattern length.

   REAL-WORLD USES
   ---------------
   * Ctrl-F search in small text widgets
   * substr checks in scripts for short inputs
   * Good when matches are rare AND the first character of
     pattern is uncommon in the text (mismatches fail early)
   ============================================================ */

#include <iostream>
#include <string>
using namespace std;

void naiveSearch(string text, string pattern) {
    int n = text.length();
    int m = pattern.length();
    int comparisons = 0;
    int matches = 0;

    cout << "Text    : " << text << "\n";
    cout << "Pattern : " << pattern << "\n";
    cout << "Matches at indices: ";

    for (int i = 0; i <= n - m; i++) {
        int j = 0;
        while (j < m && text[i + j] == pattern[j]) {
            j++; comparisons++;
        }
        if (j < m) comparisons++;       // the failing comparison
        if (j == m) { cout << i << " "; matches++; }
    }
    cout << "\nMatches found: " << matches
         << "  Total char comparisons: " << comparisons << "\n\n";
}

int main() {
    naiveSearch("AABAACAADAABAABA", "AABA");
    /* expected matches: 0, 9, 12 */

    naiveSearch("ABABABABCABABABAB", "ABABCAB");

    /* WORST-CASE-LIKE input */
    naiveSearch("AAAAAAAAAAAAAAB",  "AAAAB");

    /* edge cases */
    naiveSearch("HELLO WORLD", "");      // empty pattern (often matches all positions)
    naiveSearch("", "ABC");              // empty text
    return 0;
}

/* ============================================================
   LINE-BY-LINE EXPLANATION
   ------------------------
   Outer loop: i ranges from 0 to n-m. We don't go past n-m
   because there wouldn't be m more characters left.

   Inner while: walks j from 0 to m-1, comparing
   text[i+j] vs pattern[j]. Stops on first mismatch.

   If j reaches m, all m chars matched -> report position i.
   Otherwise increment i by 1 (no smart shift) and retry.

   ============================================================
   VIVA QUESTIONS
   --------------
   Q1. When is naive SURPRISINGLY FAST?
   A.  When mismatches happen EARLY. If the first character of
       the pattern is rare in the text, we skip past most shifts
       after one comparison. Real English text often satisfies
       this -- naive runs in ~O(n) practical time.

   Q2. When does it hit WORST CASE?
   A.  On highly repetitive text. Classic example: text =
       'AAAAAA...AB', pattern = 'AAAB'. At each shift we
       compare 4 chars, mismatching only on the last -- O(n*m)
       total.

   Q3. How to improve it?
   A.  KMP: precompute the LPS (failure) array for O(n+m).
       Boyer-Moore: skip multiple characters via bad-character
       and good-suffix rules -- sublinear average.
       Rabin-Karp: rolling hash for multi-pattern matching.

   Q4. Why no preprocessing?
   A.  By design -- this is the BASELINE. Any preprocessing
       turns it into KMP/BM/RK. The trade-off is preprocessing
       time vs search time.

   Q5. Time complexity in BIG-Theta?
   A.  Worst case Theta(n*m), best case Theta(n). Average case
       on RANDOM text: roughly Theta(n + m) -- much better than
       worst.

   ============================================================
   CROSS QUESTIONS
   ---------------
   CQ1. What if the alphabet is binary?
        Then mismatches happen less often (each char-match has
        probability 1/2). Worst case still O(n*m), but average
        is slower than on English text.

   CQ2. What if the pattern is repeated MANY TIMES in the text?
        Naive runs ~O(n) for the search part of each match plus
        O(m) for the match verification. Reporting all matches
        takes O(n + k*m) where k is the number of matches.

   CQ3. Why does the inner loop stop on mismatch?
        Because once any character differs, the pattern can't
        match here. We move to the next shift position.

   CQ4. Can we GO BACK and re-use comparisons?
        Naive doesn't -- it discards all comparison information
        on mismatch and re-compares from scratch. KMP's BIG IDEA
        is to reuse that info via the LPS array.

   CQ5. Is naive USED in real systems?
        Yes -- for short patterns in short texts, the
        preprocessing overhead of KMP/BM isn't worth it. Many
        languages' built-in find() uses naive for short cases
        and switches to BM-like for long.
   ============================================================ */
