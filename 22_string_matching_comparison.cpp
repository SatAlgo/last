/* ============================================================
   TOPIC : STRING MATCHING -- ALGORITHMS COMPARED SIDE BY SIDE
   PARADIGM : Comparison
   ============================================================

   This file runs all FOUR string matching algorithms (Naive,
   KMP, Rabin-Karp, Boyer-Moore) on the SAME inputs and prints
   a comparison table:
     - char/hash comparisons used
     - matches found

   And prints a CONCEPTUAL comparison table covering:
     - preprocessing time
     - search time (best / avg / worst)
     - space
     - when to use which

   This is the file to refer to when an examiner asks 'compare
   KMP with Rabin-Karp and Boyer-Moore'.

   ============================================================ */

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;

/* ===== NAIVE =============================================== */
int naive(const string& text, const string& pattern, int& matches) {
    int n = text.size(), m = pattern.size();
    int cmp = 0; matches = 0;
    for (int i = 0; i <= n - m; i++) {
        int j = 0;
        while (j < m && text[i+j] == pattern[j]) { j++; cmp++; }
        if (j < m) cmp++;
        if (j == m) matches++;
    }
    return cmp;
}

/* ===== KMP ================================================= */
void computeLPS(const string& pat, vector<int>& lps) {
    int m = pat.size(), len = 0; lps[0] = 0; int i = 1;
    while (i < m) {
        if (pat[i] == pat[len]) { len++; lps[i++] = len; }
        else if (len) len = lps[len-1];
        else lps[i++] = 0;
    }
}
int kmp(const string& text, const string& pattern, int& matches) {
    int n = text.size(), m = pattern.size();
    vector<int> lps(m, 0); computeLPS(pattern, lps);
    int i = 0, j = 0; int cmp = 0; matches = 0;
    while (i < n) {
        cmp++;
        if (pattern[j] == text[i]) { i++; j++; }
        if (j == m) { matches++; j = lps[j-1]; }
        else if (i < n && pattern[j] != text[i]) {
            if (j) j = lps[j-1]; else i++;
        }
    }
    return cmp;
}

/* ===== RABIN-KARP ========================================== */
int rabinKarp(const string& text, const string& pattern, int& matches) {
    const int d = 256, q = 101;
    int n = text.size(), m = pattern.size();
    int p = 0, t = 0, h = 1; int cmp = 0; matches = 0;
    for (int i = 0; i < m - 1; i++) h = (h * d) % q;
    for (int i = 0; i < m; i++) {
        p = (d * p + pattern[i]) % q;
        t = (d * t + text[i])    % q;
    }
    for (int i = 0; i <= n - m; i++) {
        cmp++;                                     // hash comparison
        if (p == t) {
            int j;
            for (j = 0; j < m; j++) {
                cmp++;                             // char comparison
                if (text[i+j] != pattern[j]) break;
            }
            if (j == m) matches++;
        }
        if (i < n - m) {
            t = (d * (t - text[i] * h) + text[i+m]) % q;
            if (t < 0) t += q;
        }
    }
    return cmp;
}

/* ===== BOYER-MOORE (bad-char only) ========================= */
int boyerMoore(const string& text, const string& pattern, int& matches) {
    const int NCH = 256;
    int n = text.size(), m = pattern.size();
    int badchar[NCH];
    for (int i = 0; i < NCH; i++) badchar[i] = -1;
    for (int i = 0; i < m; i++) badchar[(int)pattern[i]] = i;
    int s = 0; int cmp = 0; matches = 0;
    while (s <= n - m) {
        int j = m - 1;
        while (j >= 0 && pattern[j] == text[s+j]) { j--; cmp++; }
        if (j < 0) {
            matches++;
            s += (s + m < n) ? m - badchar[(int)text[s+m]] : 1;
        } else {
            cmp++;
            s += max(1, j - badchar[(int)text[s+j]]);
        }
    }
    return cmp;
}

/* ----- run all four and print comparison table ------------ */
void runAndCompare(const string& text, const string& pattern) {
    cout << "Text    : " << text << "\n";
    cout << "Pattern : " << pattern
         << "   (n=" << text.size() << ", m=" << pattern.size() << ")\n\n";

    int mN, mK, mR, mB;
    int cN = naive(text,      pattern, mN);
    int cK = kmp(text,        pattern, mK);
    int cR = rabinKarp(text,  pattern, mR);
    int cB = boyerMoore(text, pattern, mB);

    cout << "+-----------------+----------+---------+\n";
    cout << "| Algorithm       | Comparis | Matches |\n";
    cout << "+-----------------+----------+---------+\n";
    cout << "| Naive           |   " << setw(6) << cN << " |   " << setw(5) << mN << " |\n";
    cout << "| KMP             |   " << setw(6) << cK << " |   " << setw(5) << mK << " |\n";
    cout << "| Rabin-Karp      |   " << setw(6) << cR << " |   " << setw(5) << mR << " |\n";
    cout << "| Boyer-Moore     |   " << setw(6) << cB << " |   " << setw(5) << mB << " |\n";
    cout << "+-----------------+----------+---------+\n\n";
}

int main() {
    cout << "==========================================================\n";
    cout << "   STRING MATCHING ALGORITHMS -- EMPIRICAL COMPARISON\n";
    cout << "==========================================================\n\n";

    cout << "--- TEST 1 : ordinary text ---\n";
    runAndCompare("AABAACAADAABAABA", "AABA");

    cout << "--- TEST 2 : worst case for naive ---\n";
    runAndCompare("AAAAAAAAAAAAAAB", "AAAAB");

    cout << "--- TEST 3 : large alphabet (BM shines) ---\n";
    runAndCompare("THEQUICKBROWNFOXJUMPSOVERTHELAZYDOG", "FOX");

    cout << "--- TEST 4 : multiple non-overlapping matches ---\n";
    runAndCompare("ABABABABABABABAB", "ABAB");

    cout << "\n==========================================================\n";
    cout << "   CONCEPTUAL COMPARISON TABLE\n";
    cout << "==========================================================\n";

    cout << "\n";
    cout << "+--------------+---------------+----------------+----------------+--------------------+\n";
    cout << "| Algorithm    | Preprocessing | Search (avg)   | Search (worst) | Space              |\n";
    cout << "+--------------+---------------+----------------+----------------+--------------------+\n";
    cout << "| Naive        | None          | O(n*m)         | O(n*m)         | O(1)               |\n";
    cout << "| KMP          | O(m)          | O(n + m)       | O(n + m)       | O(m)               |\n";
    cout << "| Rabin-Karp   | O(m)          | O(n + m)       | O(n*m)         | O(1)               |\n";
    cout << "| Boyer-Moore  | O(m + sigma)  | O(n / m)       | O(n*m)*        | O(sigma)           |\n";
    cout << "+--------------+---------------+----------------+----------------+--------------------+\n";
    cout << "  *with both heuristics (bad-char + good-suffix), worst case improves to O(n + m)\n";
    cout << "  sigma = alphabet size (256 for ASCII)\n\n";

    cout << "WHEN TO USE WHAT\n";
    cout << "----------------\n";
    cout << " * Naive       : tiny inputs, one-off short patterns; no setup\n";
    cout << " * KMP         : guaranteed O(n+m) worst case; small alphabets;\n";
    cout << "                 streaming data where text pointer mustn't go back\n";
    cout << " * Rabin-Karp  : MULTI-pattern search (k patterns -> O(n + k*m));\n";
    cout << "                 plagiarism detection, fingerprinting\n";
    cout << " * Boyer-Moore : LONG patterns, LARGE alphabet (English/UTF-8);\n";
    cout << "                 grep, file searches -- typically fastest in real use\n";
    return 0;
}

/* ============================================================
   EXPLANATION
   -----------
   This file is a 'comparison harness' rather than a learning
   tutorial. The previous four files (18, 19, 20, 21) each
   have their full tutorial.

   ============================================================
   VIVA QUESTIONS
   --------------
   Q1. Which algorithm is BEST 'in practice'?
   A.  Boyer-Moore (or one of its descendants like BM-Horspool
       or Sunday) for English text. KMP for streaming or small
       alphabets. Rabin-Karp for multi-pattern.

   Q2. Why is Rabin-Karp's WORST CASE same as Naive?
   A.  When every hash matches (lots of collisions), each
       window triggers full O(m) verification. So O(n*m). With
       a good random hash, the probability is tiny.

   Q3. Which has the BEST AVERAGE case?
   A.  Boyer-Moore -- sublinear O(n/m) on large alphabets
       because the bad-char shifts are usually near m.

   Q4. Which has DETERMINISTIC linear time?
   A.  Only KMP. RK is expected-linear; BM is average-linear.
       KMP's O(n+m) holds for ALL inputs.

   Q5. Why do we still TEACH naive?
   A.  It's the BASELINE that motivates the others. Each
       smarter algorithm exploits a different inefficiency in
       naive: KMP reuses comparisons; RK uses hashes; BM skips
       chunks of text.

   ============================================================
   CROSS QUESTIONS
   ---------------
   CQ1. Can these algorithms be combined?
        Yes! Real-world tools use hybrid heuristics. GNU grep
        switches between BM-like for long patterns and naive
        for short.

   CQ2. What about REGEX matching?
        Generalizes string matching to a regular language --
        compiled into an NFA (Thompson construction) and run
        on the text. O(n*m) in the worst case for general
        regex, O(n) for simpler features.

   CQ3. Suffix-based methods?
        Suffix arrays / suffix trees / FM-index preprocess the
        TEXT (not the pattern), enabling O(m) queries for ANY
        pattern. Ideal when the SAME text is searched many
        times (e.g. genome).

   CQ4. Approximate matching?
        Allow up to k edit-distance differences -- Bitap
        algorithm and Wu-Manber are popular. Used in 'grep -E
        with errors' and DNA analysis.

   CQ5. Parallel string matching?
        Split text into overlapping chunks (overlap = m-1 to
        not miss matches at boundaries) and run any algorithm
        in parallel. Near-linear speedup possible on multi-core.
   ============================================================ */
