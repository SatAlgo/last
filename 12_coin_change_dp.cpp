/* ============================================================
   TOPIC : COIN CHANGE  (Dynamic Programming version)
   PARADIGM : Dynamic Programming
   ============================================================

   CONCEPT
   -------
   Find the MINIMUM coins needed to make a target amount,
   working for ANY coin system (canonical or not). Build up
   dp[i] = min coins needed to make amount i. For each amount
   i and each coin c with c <= i:
       dp[i] = min(dp[i],  dp[i - c] + 1)
   Base: dp[0] = 0 (zero coins to make zero).

   BRUTE FORCE
   -----------
   Pure recursion: minCoins(amount) = 1 + min(minCoins(amount-c))
   over each coin c. Exponential because of repeated
   subproblems. DP MEMOIZES them.

   COMPLEXITY
   ----------
       Best     : O(n * amount)
       Average  : O(n * amount)
       Worst    : O(n * amount)
       Space    : O(amount)            (1D dp)
       PSEUDO-POLYNOMIAL: depends on amount's VALUE, not bits.

   RECONSTRUCTION
   --------------
   Keep a parent[] array storing WHICH coin was picked to
   reach amount i optimally. Backtrack from parent[amount]
   down to 0 to list the coins.

   REAL-WORLD USES
   ---------------
   * Currency systems with unusual denominations
   * 'Word break' problem (string into dictionary words)
   * Stock cutting (paper / steel rolls minimizing waste)
   * Change-making in self-checkout for non-standard coin sets
   ============================================================ */

#include <iostream>
#include <vector>
#include <climits>
using namespace std;

void coinChangeDP(vector<int>& coins, int amount) {
    vector<int> dp(amount + 1, INT_MAX);
    vector<int> parent(amount + 1, -1);          // for reconstruction
    dp[0] = 0;

    for (int i = 1; i <= amount; i++) {
        for (int c : coins) {
            if (c <= i && dp[i - c] != INT_MAX
                       && dp[i - c] + 1 < dp[i]) {
                dp[i]     = dp[i - c] + 1;
                parent[i] = c;
            }
        }
    }

    cout << "Coins      : ";
    for (int c : coins) cout << c << " ";
    cout << "\nAmount     : " << amount << "\n";

    if (dp[amount] == INT_MAX) {
        cout << "Result     : IMPOSSIBLE\n\n";
        return;
    }
    cout << "Min coins  : " << dp[amount] << "\n";

    /* reconstruct picks */
    vector<int> picks;
    int cur = amount;
    while (cur > 0) {
        picks.push_back(parent[cur]);
        cur -= parent[cur];
    }
    cout << "Pick list  : ";
    for (int x : picks) cout << x << " ";
    cout << "\n\n";
}

int main() {
    /* Same NON-CANONICAL case greedy failed on */
    vector<int> coins1 = {1, 3, 4};
    coinChangeDP(coins1, 6);          /* expects 2 coins: 3+3 */

    vector<int> coins2 = {1, 2, 5};
    coinChangeDP(coins2, 11);         /* expects 3 coins: 5+5+1 */

    vector<int> coins3 = {2, 4, 6};
    coinChangeDP(coins3, 7);          /* impossible */

    vector<int> coins4 = {1};
    coinChangeDP(coins4, 100);        /* 100 coins of 1 */
    return 0;
}

/* ============================================================
   LINE-BY-LINE EXPLANATION
   ------------------------
   dp[0] = 0: base. Zero coins make zero.
   dp[i] = INT_MAX: 'unknown / impossible' marker.

   Outer loop fills dp[1] .. dp[amount] in order. For each i,
   try every coin c. If c <= i and dp[i-c] is reachable, we
   could reach i by adding one c-coin to that subproblem's
   solution. Update dp[i] if this is fewer coins.

   parent[i] stores which coin was used in the best solution
   for amount i, enabling reconstruction.

   Reconstruction loop: start from amount, jump back by
   parent[cur], record the coin, repeat until cur = 0.

   ============================================================
   VIVA QUESTIONS
   --------------
   Q1. Why is this called 'pseudo-polynomial'?
   A.  The complexity O(n * amount) depends on the VALUE of
       amount, not its bit-size. If amount has k bits, input
       size is k but runtime is O(n * 2^k) -- exponential in
       input size. So 'pseudo-polynomial' = polynomial in
       value, exponential in bit-length.

   Q2. How do you reconstruct WHICH coins were used?
   A.  Keep a parent[] array. parent[i] stores the coin used to
       achieve dp[i]. Walk back: cur -> cur - parent[cur] ->
       ... until cur = 0.

   Q3. Memoization vs Tabulation -- which is better here?
   A.  Tabulation (bottom-up) avoids recursion overhead and
       stack issues. Memoization (top-down) is easier to write
       and only computes needed subproblems. For coin change
       with all subproblems reachable, tabulation is typically
       preferred.

   Q4. What if no combination works?
   A.  dp[amount] stays INT_MAX. Return -1 (or 'impossible').
       Example: coins = {2}, amount = 3 -- no way.

   Q5. Difference vs the 'count number of ways' version?
   A.  'Min coins': dp[i] = min(dp[i-c]+1). Loop order: outer
       amount, inner coin.
       'Count ways': dp[i] += dp[i-c]. Outer coin, inner amount
       to avoid duplicates. SAME table, different semantics.

   ============================================================
   CROSS QUESTIONS
   ---------------
   CQ1. Space optimization?
        Already O(amount) -- can't really go lower because the
        last 'amount' value is asked at the very end. (Some DP
        problems can compress further; this one can't.)

   CQ2. Why is this NP-hard for arbitrary representations?
        Because amount can be very large (input size = log2(amount)
        bits). 0/1 KNAPSACK is similarly NP-complete and reduces
        to this. But pseudo-polynomial DP exists.

   CQ3. How would you handle the BOUNDED coin variant (limited
        supply of each)?
        Make a 2D DP: dp[i][a] = min coins using first i coin
        types with limit, to make amount a. Transitions
        consider 0..count[i] copies of coin i. O(n * amount *
        max_count).

   CQ4. Greedy vs DP -- empirical compare?
        For canonical sets, greedy is O(n log n + amount/min).
        DP is O(n * amount). DP is slower but always correct.
        Real cash systems use greedy because denominations were
        DESIGNED to make it correct.

   CQ5. Iterative dependency order?
        dp[i] depends on dp[i-c] for various c. As long as we
        compute dp in INCREASING amount order, dependencies are
        satisfied -- left to right.
   ============================================================ */
