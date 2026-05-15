/* ============================================================
   TOPIC : 0/1 KNAPSACK  (Dynamic Programming)
   PARADIGM : Dynamic Programming
   ============================================================

   CONCEPT
   -------
   Each item is INDIVISIBLE: take it fully or skip it. Pick a
   subset of items maximizing total value while total weight
   stays within capacity W.

   dp[i][w] = maximum value achievable using the first i items
              with capacity w.
   Transition:
       if wt[i-1] > w:    dp[i][w] = dp[i-1][w]    (must skip)
       else:              dp[i][w] = max(dp[i-1][w],
                                          dp[i-1][w-wt[i-1]] + val[i-1])
                          (skip or take)
   Base: dp[0][w] = 0 (no items -> 0 value).

   This file shows BOTH the standard 2D version AND the
   space-optimized 1D version.

   BRUTE FORCE
   -----------
   Try all 2^n subsets, take the one with max value within
   capacity. O(2^n) time. DP reuses subproblem results to drop
   to O(n * W).

   COMPLEXITY
   ----------
       Best     : O(n * W)
       Average  : O(n * W)
       Worst    : O(n * W)
       Space    : O(n * W)   2D version
                  O(W)       1D space-optimized
       PSEUDO-POLYNOMIAL: depends on numerical value W.

   REAL-WORLD USES
   ---------------
   * Cargo loading for airplanes / ships -- discrete items
   * Investment portfolios with indivisible assets (whole
     shares, real estate)
   * Resource allocation in cloud computing (which VMs on
     which host)
   * Project selection under budget (capital budgeting)
   * Cutting stock (paper, glass, steel) into pieces
   ============================================================ */

#include <iostream>
#include <vector>
using namespace std;

/* --- BASIC: 2D DP with reconstruction ---------------------- */
int knapsack2D(int W, vector<int>& wt, vector<int>& val, int n) {
    vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));

    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            if (wt[i-1] <= w)
                dp[i][w] = max(dp[i-1][w],
                               dp[i-1][w - wt[i-1]] + val[i-1]);
            else
                dp[i][w] = dp[i-1][w];
        }
    }

    /* reconstruct which items were chosen */
    cout << "Items chosen (2D version): ";
    int w = W;
    for (int i = n; i > 0; i--) {
        if (dp[i][w] != dp[i-1][w]) {
            cout << "item" << i << "(v=" << val[i-1]
                 << ",w=" << wt[i-1] << ") ";
            w -= wt[i-1];
        }
    }
    cout << "\n";
    return dp[n][W];
}

/* --- ADVANCED: space-optimized 1D DP ----------------------- */
int knapsack1D(int W, vector<int>& wt, vector<int>& val, int n) {
    vector<int> dp(W + 1, 0);

    /* CRUCIAL: iterate weight from HIGH to LOW so the value
       used on the right side of '=' refers to dp[i-1][...],
       not the just-updated dp[i][...]. */
    for (int i = 0; i < n; i++) {
        for (int w = W; w >= wt[i]; w--) {
            dp[w] = max(dp[w], dp[w - wt[i]] + val[i]);
        }
    }
    return dp[W];
}

int main() {
    vector<int> val = {60, 100, 120};
    vector<int> wt  = {10,  20,  30};
    int W = 50, n = 3;

    cout << "Capacity = " << W << "\n";
    cout << "Items (value,weight): ";
    for (int i = 0; i < n; i++)
        cout << "(" << val[i] << "," << wt[i] << ") ";
    cout << "\n\n";

    int ans2D = knapsack2D(W, wt, val, n);
    cout << "Max value (2D)  : " << ans2D << "\n";

    int ans1D = knapsack1D(W, wt, val, n);
    cout << "Max value (1D)  : " << ans1D << "  (must equal 2D)\n";
    /* Expected: 220  (items 2 and 3 -- weights 20+30, values 100+120) */

    /* edge cases */
    vector<int> ev = {10}, ew = {15};
    cout << "\nSingle item too heavy (W=10, item weight 15) : "
         << knapsack1D(10, ew, ev, 1) << " (should be 0)\n";

    vector<int> empty_v, empty_w;
    cout << "No items : " << knapsack1D(100, empty_w, empty_v, 0)
         << " (should be 0)\n";
    return 0;
}

/* ============================================================
   LINE-BY-LINE EXPLANATION
   ------------------------
   2D version:
     dp[i][w] only depends on dp[i-1][...].  Fill row-by-row.
     If wt[i-1] > w: cannot take item i, just copy dp[i-1][w].
     Else: best of (skip = dp[i-1][w]) and
                   (take = dp[i-1][w-wt[i-1]] + val[i-1]).

   Reconstruction:
     Walk from dp[n][W] backwards. If dp[i][w] differs from
     dp[i-1][w], item i was taken; decrement w by wt[i-1].
     Else item i was skipped; keep w.

   1D version:
     dp[w] alone. KEY: iterate w from HIGH to LOW. This ensures
     dp[w-wt[i]] still refers to the PREVIOUS iteration (i-1),
     not the current one -- preserving 0/1 semantics. If you
     iterate LOW-to-HIGH instead, you get UNBOUNDED knapsack
     (multiple copies allowed).

   ============================================================
   VIVA QUESTIONS
   --------------
   Q1. Why can't greedy work here?
   A.  Items are INDIVISIBLE. Highest value/weight item might
       leave capacity that can't be filled. Example: W=50,
       items (60,10),(100,20),(120,30). Greedy by ratio takes
       items 1 + 2 = 160. DP picks items 2+3 = 220.

   Q2. Why does 1D iteration go BACKWARD?
   A.  To preserve 0/1 semantics. dp[w-wt[i]] must reference
       the PREVIOUS row (without item i). Backward iteration
       ensures we use the not-yet-updated values. Forward
       would give UNBOUNDED knapsack (item used multiple times).

   Q3. Is O(n*W) truly polynomial?
   A.  No -- pseudo-polynomial. W is a VALUE, not the input
       size. If W has k bits, input size is O(k) but runtime is
       O(n * 2^k). 0/1 knapsack is NP-Complete.

   Q4. Difference between 0/1 and Unbounded knapsack?
   A.  0/1: each item available once. Unbounded: each item
       available infinite times. The DP recurrence differs only
       in iteration direction for the 1D form. (Or different
       base for the 2D form.)

   Q5. Recurrence in plain English?
   A.  'Best value with first i items and capacity w' = better
       of 'skip item i' or 'take item i and add its value to
       best with first i-1 items and remaining capacity'.

   ============================================================
   CROSS QUESTIONS
   ---------------
   CQ1. Why does this beat brute force exponentially?
        Brute force tries 2^n subsets. DP overlaps subproblems
        -- the same 'first i items with capacity w' state
        appears in many subsets but is computed only once.

   CQ2. Can we do better than O(n*W) for special structures?
        For 'meet in the middle': split items into two halves,
        enumerate 2^(n/2) subsets of each, combine. O(2^(n/2))
        instead of O(2^n). Useful when W is huge but n is
        moderate.

   CQ3. What about FRACTIONAL knapsack?
        That allows real-number fractions -- becomes GREEDY
        (sort by ratio, fill).  Different problem entirely.

   CQ4. How to handle MULTIPLE knapsacks?
        Way harder -- becomes a packing problem. For 2
        knapsacks: dp[i][w1][w2]. For k knapsacks: dp[i][w1]
        [w2]...[wk] -- exponential in k.

   CQ5. Approximation algorithms?
        Fully Polynomial Time Approximation Scheme (FPTAS)
        exists: scale values by factor 1/eps and run DP on
        scaled values -- guaranteed (1-eps) of optimum in
        polynomial time.
   ============================================================ */
