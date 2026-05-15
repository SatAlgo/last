/* ============================================================
   TOPIC : BOYER-MOORE STRING MATCHING
   PARADIGM : String Matching
   ============================================================

   CONCEPT
   -------
   Compare pattern with text RIGHT-TO-LEFT. On mismatch, use
   the BAD CHARACTER HEURISTIC to skip multiple positions:
   shift the pattern so the mismatching text character aligns
   with its LAST occurrence in the pattern (or past the
   pattern if the character doesn't appear).

   The full algorithm also uses the GOOD SUFFIX HEURISTIC; this
   implementation uses bad character only for clarity. Together
   they make Boyer-Moore SUBLINEAR on AVERAGE for large
   alphabets -- the typical pattern doesn't even examine every
   text character.

   BRUTE FORCE: naive O(n*m).

   COMPLEXITY (bad-char only)
   --------------------------
       Best     : O(n/m)            VERY fast when alphabet is large
       Average  : O(n)
       Worst    : O(n*m)            pathological inputs
       Space    : O(alphabet size)  = O(256) for ASCII

   With both heuristics, worst case improves to O(n+m).

   REAL-WORLD USES
   ---------------
   * grep, ripgrep, ack -- standard Unix search tools use BM
     variants (Boyer-Moore-Horspool / Sunday)
   * Code editors searching for tokens in huge source files
   * GNU grep is famous for using BM with heavy optimization
   * Anti-virus signature scanning (huge fixed pattern set)
   ============================================================ */

#include <iostream>
#include <string>
#include <climits>
using namespace std;

#define NO_OF_CHARS 256

/* badChar[c] = last index of c in pattern, or -1 if absent */
void badCharHeuristic(string str, int size, int badchar[NO_OF_CHARS]) {
    for (int i = 0; i < NO_OF_CHARS; i++) badchar[i] = -1;
    for (int i = 0; i < size; i++) badchar[(int)str[i]] = i;
}

void boyerMoore(string text, string pattern) {
    int n = text.length();
    int m = pattern.length();
    if (m == 0 || n < m) { cout << "No matches.\n"; return; }

    int badchar[NO_OF_CHARS];
    badCharHeuristic(pattern, m, badchar);

    cout << "Text    : " << text << "\n";
    cout << "Pattern : " << pattern << "\n";
    cout << "Matches at indices: ";

    int s = 0;                  // current shift of pattern
    int comparisons = 0;
    int totalSkip = 0;
    while (s <= n - m) {
        int j = m - 1;          // compare RIGHT-TO-LEFT
        while (j >= 0 && pattern[j] == text[s + j]) { j--; comparisons++; }
        if (j < 0) {
            cout << s << " ";
            /* shift so the next char of text aligns with last occurrence
               in pattern of that char (or 1 past end if not present) */
            int shift = (s + m < n) ? m - badchar[text[s + m]] : 1;
            totalSkip += shift;
            s += shift;
        } else {
            comparisons++;
            int shift = max(1, j - badchar[(int)text[s + j]]);
            totalSkip += shift;
            s += shift;
        }
    }
    cout << "\nComparisons : " << comparisons
         << "   Total shift distance : " << totalSkip << "\n\n";
}

int main() {
    boyerMoore("ABAAABCD", "ABC");
    /* expected match at 4 */

    boyerMoore("AABAACAADAABAABA", "AABA");
    /* expected matches at 0, 9, 12 */

    boyerMoore("HEREISASIMPLESEARCH", "SEARCH");
    /* expected match at 13 */

    /* large alphabet, big jumps */
    boyerMoore("THEQUICKBROWNFOXJUMPS", "JUMPS");
    return 0;
}

/* ============================================================
   LINE-BY-LINE EXPLANATION
   ------------------------
   badCharHeuristic():
     Initialize all entries to -1 (char not in pattern). Then
     for each position i in pattern, set badchar[pattern[i]] = i.
     The LATER occurrence overwrites the earlier -- we want
     the LAST (rightmost) occurrence.

   boyerMoore():
     s = shift of pattern relative to text (where the pattern
     starts).
     j = index inside pattern -- starts at m-1 (last char).
     We compare RIGHT-TO-LEFT.

     If j reaches -1, full pattern matched -> report s, then
     shift forward.

     On mismatch at pattern[j] != text[s+j]:
       shift = max(1, j - badchar[text[s+j]])
       This aligns the mismatching text character with its
       last occurrence in the pattern. If the character isn't
       in pattern, badchar = -1 and we shift past it (j - (-1)
       = j+1).
       max(1, ...) ensures we always make at least 1 step of
       progress.

   ============================================================
   VIVA QUESTIONS
   --------------
   Q1. Why does Boyer-Moore compare RIGHT-TO-LEFT?
   A.  So that a mismatch at the END of the pattern can shift
       the pattern by up to m positions in one step. Going
       left-to-right doesn't give that big-jump advantage.

   Q2. What is the BAD CHARACTER HEURISTIC?
   A.  On mismatch at text[s+j], find the LAST occurrence of
       that character in pattern. Shift so they align. If the
       character doesn't appear in pattern, shift past it.

   Q3. What is the GOOD SUFFIX HEURISTIC (briefly)?
   A.  If we matched some suffix of the pattern before mismatch,
       shift so the matched suffix re-aligns with an earlier
       (or different) occurrence of that suffix in the pattern.
       Used together with bad-char (take the LARGER shift).

   Q4. Why is BM SUBLINEAR on average?
   A.  On large alphabets, the bad-char shift is typically near
       m. So we examine roughly n/m text characters. Hence
       O(n/m) average for moderate patterns over large
       alphabets.

   Q5. When does worst case happen?
   A.  Pattern and text both repetitive over a small alphabet,
       e.g. text='AAAA...A', pattern='AAAB'. Bad-char shifts
       are only 1. The good-suffix heuristic helps but the
       full Boyer-Moore worst case (bad-char only) is O(n*m).

   ============================================================
   CROSS QUESTIONS
   ---------------
   CQ1. Boyer-Moore-Horspool variant?
        Uses only the LAST character of the window for the
        shift rule (not the mismatch position). Simpler,
        slightly worse worst case, often faster in practice on
        natural-language text.

   CQ2. SUNDAY variant?
        Compares left-to-right but uses the character AT
        position s+m (just after the window) for the shift.
        Easy to implement and often the fastest in practice
        for English text.

   CQ3. Why is good-suffix more complex than bad-character?
        Bad-char is a simple table indexed by character. Good-
        suffix involves precomputing, for every possible suffix
        of the pattern, the next earlier occurrence -- needs a
        Z-function-like preprocess.

   CQ4. BM vs KMP -- both worst case O(n+m), why use BM?
        BM is FASTER IN PRACTICE on most real text because its
        sublinear average is achieved easily. KMP makes one
        comparison per text character at minimum. BM often
        skips text entirely.

   CQ5. Memory considerations?
        Bad-char table is O(alphabet size). For UNICODE, that's
        up to 1M entries -- prohibitive. Most real
        implementations use hash maps or restrict to ASCII.
   ============================================================ */
