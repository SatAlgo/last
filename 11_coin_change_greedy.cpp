/* ============================================================
   TOPIC : COIN CHANGE  (Greedy version)
   PARADIGM : Greedy
   ============================================================

   CONCEPT
   -------
   Make a target amount using the FEWEST coins. Greedy: at
   each step, pick the largest coin that doesn't overshoot,
   then continue with the remaining amount.

   This works ONLY for 'canonical' coin systems like
   {1, 5, 10, 25, 50, 100} (US Dollar) or {1, 2, 5, 10, 20,
   50, 100, 500, 2000} (Indian Rupee). It FAILS for systems
   like {1, 3, 4}.

   This file demonstrates BOTH the success case and the
   failure case side by side -- the comparison is the point.

   BRUTE FORCE
   -----------
   Try every combination (multinomial coin counts) that sums
   to the target -- exponential. Greedy is the optimization
   that works for canonical systems.

   COMPLEXITY
   ----------
       Best     : O(n log n)   sorting the coins
       Average  : O(n log n)
       Worst    : O(n log n + amount/min_coin)
       Space    : O(1) plus the result list
       (For canonical systems, far less coins are used.)

   REAL-WORLD USES
   ---------------
   * ATM cash dispensing -- canonical denominations work
   * Vending machine change
   * Currency exchange systems
   * Quick approximation when speed matters more than
     guaranteed optimum
   ============================================================ */

#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

void coinChangeGreedy(vector<int> coins, int amount, const string& label) {
    int original = amount;
    sort(coins.begin(), coins.end(), greater<int>());

    vector<int> used;
    for (int c : coins) {
        while (amount >= c) {
            amount -= c;
            used.push_back(c);
        }
    }

    cout << label << "\n";
    cout << "  Coins available : ";
    for (int c : coins) cout << c << " ";
    cout << "\n  Target amount   : " << original << "\n";
    if (amount != 0) {
        cout << "  Result          : EXACT CHANGE NOT POSSIBLE\n\n";
        return;
    }
    cout << "  Coins picked    : ";
    for (int x : used) cout << x << " ";
    cout << "(total " << used.size() << " coins)\n\n";
}

int main() {
    /* CASE A: Canonical Indian Rupee coin set -- greedy WORKS */
    coinChangeGreedy({1, 2, 5, 10, 20, 50, 100, 500, 2000}, 93,
                     "[A] Indian Rupee (canonical) for 93:");

    /* CASE B: US Dollar style -- greedy WORKS */
    coinChangeGreedy({1, 5, 10, 25}, 30,
                     "[B] US Dollar (canonical) for 30 cents:");

    /* CASE C: Non-canonical {1,3,4} -- greedy FAILS for 6.
              Greedy gives 4+1+1 = 3 coins.
              DP optimal = 3+3 = 2 coins. */
    coinChangeGreedy({1, 3, 4}, 6,
                     "[C] Non-canonical {1,3,4} for 6  (greedy says 3 coins, optimum is 2):");

    /* CASE D: Impossible target */
    coinChangeGreedy({2, 4, 6}, 7,
                     "[D] Even coins only, odd target 7:");

    cout << "Conclusion: greedy is only safe for CANONICAL coin systems.\n";
    cout << "For arbitrary systems, use Dynamic Programming.\n";
    return 0;
}

/* ============================================================
   LINE-BY-LINE EXPLANATION
   ------------------------
   sort(..., greater<int>()) puts coins in DESCENDING order so
   we encounter the largest first.

   For each coin c, while the remaining amount allows, subtract
   c and record it. Move on when c overshoots.

   After processing all coins, if amount != 0, exact change is
   impossible with the given coin set (or greedy can't find it).

   ============================================================
   VIVA QUESTIONS
   --------------
   Q1. When does the greedy approach FAIL?
   A.  When the coin system is NON-CANONICAL. Example: coins
       {1,3,4} for target 6. Greedy: 4+1+1 = 3 coins. Optimal:
       3+3 = 2 coins. Greedy was locked into the 4, leaving a
       gap that couldn't be filled optimally.

   Q2. How do you check if a coin system is canonical?
   A.  Pearson's algorithm checks it in O(n^3) where n is the
       number of denominations. There's no simple closed-form
       rule. Most real currencies (USD, INR, EUR) happen to be
       canonical -- not coincidence; they were designed that way.

   Q3. What's the fallback when greedy fails?
   A.  Dynamic Programming (file 12). O(n * amount) time,
       guaranteed optimal regardless of coin system.

   Q4. Time complexity?
   A.  O(n log n) for sorting plus O(amount / min_coin) for the
       loop. The sort dominates for small amounts; for huge
       amounts the loop dominates.

   Q5. Can greedy fail even for a CANONICAL coin set?
   A.  No -- canonicality is precisely the property that
       greedy succeeds. The hard part is determining whether
       a coin set is canonical.

   ============================================================
   CROSS QUESTIONS
   ---------------
   CQ1. Why is the Indian Rupee system canonical?
        Each denomination is at most 2-5x the previous, with
        ratios chosen so greedy stays optimal. {1, 2, 5, 10,
        20, 50, ...} -- the pattern of 1,2,5 multiples preserves
        canonicality.

   CQ2. What if there are infinitely many copies of each coin?
        That's the default assumption here. The 'bounded coin'
        variant where you have a LIMITED supply of each coin
        is harder -- needs DP even on canonical systems.

   CQ3. Greedy choice property vs Optimal substructure here?
        Greedy choice property HOLDS on canonical sets (always
        safe to pick the biggest coin), FAILS otherwise.
        Optimal substructure ALWAYS holds (the rest of the
        change for amount-c is itself an optimal subproblem).

   CQ4. Real-world: ATMs use greedy. Is that safe?
        Yes -- ATM denomination sets are deliberately designed
        to be canonical. If a country introduced a 3-rupee
        coin, ATMs would have to switch to DP.

   CQ5. Greedy vs DP -- speed difference for large amounts?
        Greedy: O(n log n + amount / min). DP: O(n * amount).
        For amount = 10^6 and 10 coins, greedy is ~10^7 vs
        DP's 10^7. Greedy wins on cache locality.
   ============================================================ */
