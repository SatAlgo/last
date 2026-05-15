/* ============================================================
   TOPIC : SUM OF SUBSETS  (Branch & Bound)
   PARADIGM : Branch & Bound
   ============================================================

   CONCEPT
   -------
   Given an array of POSITIVE integers and a target sum, find
   all subsets that sum exactly to the target. Build a state-
   space tree where each node decides INCLUDE/EXCLUDE for an
   element. Prune with two bounds:
     (1) UPPER:  if currSum + nums[idx] > target, including
                 overshoots -- prune.
     (2) LOWER:  if currSum + remainingSum < target, even
                 taking everything left can't reach -- prune.

   Sorting the input ASCENDING makes the upper bound trigger
   earlier (smallest remaining element overshoots first).

   BRUTE FORCE
   -----------
   Enumerate all 2^n subsets and check each sum. O(2^n * n).
   B&B keeps the worst-case 2^n but in practice prunes most.

   COMPLEXITY
   ----------
       Best     : O(n)         very lucky pruning
       Average  : depends on input -- usually much less than 2^n
       Worst    : O(2^n)        no pruning
       Space    : O(n)         recursion + subset buffer

   REAL-WORLD USES
   ---------------
   * Cryptography (basis of the Merkle-Hellman knapsack
     cryptosystem; now broken but historic)
   * Resource allocation -- selecting projects whose total cost
     equals budget exactly
   * Coin change variants where you must pay EXACTLY X
   * Tax / expense matching in accounting reconciliation
   * Subset selection in research data analysis
   ============================================================ */

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void sumOfSubsets(vector<int>& nums, int idx, int currSum, int target,
                  int remainingSum, vector<int>& subset) {
    /* SUCCESS: we hit the target */
    if (currSum == target) {
        cout << "{ ";
        for (int x : subset) cout << x << " ";
        cout << "}\n";
        return;
    }
    if (idx >= (int)nums.size()) return;

    /* LOWER BOUND PRUNE: not enough remaining to reach target */
    if (currSum + remainingSum < target) return;

    /* UPPER BOUND PRUNE: including current element overshoots
       (relies on nums being sorted ascending) */
    if (currSum + nums[idx] > target) return;

    /* INCLUDE nums[idx] */
    subset.push_back(nums[idx]);
    sumOfSubsets(nums, idx + 1, currSum + nums[idx], target,
                 remainingSum - nums[idx], subset);
    subset.pop_back();

    /* EXCLUDE nums[idx] */
    sumOfSubsets(nums, idx + 1, currSum, target,
                 remainingSum - nums[idx], subset);
}

int main() {
    vector<int> nums = {5, 10, 12, 13, 15, 18};
    int target = 30;

    sort(nums.begin(), nums.end());            // ascending order helps pruning
    int totalSum = 0;
    for (int x : nums) totalSum += x;

    vector<int> subset;
    cout << "Input: { ";
    for (int x : nums) cout << x << " ";
    cout << "}  target = " << target << "\n";
    cout << "Subsets that sum to " << target << ":\n";
    sumOfSubsets(nums, 0, 0, target, totalSum, subset);

    /* Expected outputs:
       { 5 12 13 }
       { 12 18 }     <- equivalent to {18,12} due to sorted input
       Several combinations; depends on the test. */
    return 0;
}

/* ============================================================
   LINE-BY-LINE EXPLANATION
   ------------------------
   subset[]: current partial selection being explored.
   currSum: sum of subset[].
   remainingSum: sum of all elements from index idx onward.

   At each call:
     - If currSum == target, print and return -- found one.
     - If idx out of bounds, return.
     - LOWER bound prune: even adding ALL remaining can't reach
       target -> dead end.
     - UPPER bound prune: adding the smallest remaining (which
       is nums[idx] because of ascending sort) already
       overshoots -> dead end.

   Otherwise two branches:
     INCLUDE: push, recurse with new sums, pop on return.
     EXCLUDE: recurse without modifying subset; remainingSum
              still decreases (this element is now 'done').

   ============================================================
   VIVA QUESTIONS
   --------------
   Q1. Why sort the input first?
   A.  With sorted (ASCENDING) input, the smallest remaining
       element is nums[idx]. If even THAT overshoots, every
       later element would too -- the upper bound triggers
       earlier and more aggressively.

   Q2. Is subset-sum NP-Complete?
   A.  YES -- one of Karp's 21 classic NP-Complete problems.
       No polynomial-time algorithm is known. DP gives a
       pseudo-polynomial O(n * target) algorithm.

   Q3. How do the two bounds work?
   A.  UPPER bound: 'currSum + nums[idx] > target' means
       including overshoots -- prune.
       LOWER bound: 'currSum + remainingSum < target' means
       even taking everything left won't reach -- prune.

   Q4. What if we only want to COUNT subsets, not list them?
   A.  Use DP: dp[i][s] = number of ways using first i elements
       to reach sum s. O(n * target) time and space. The B&B
       approach is for ENUMERATION.

   Q5. Why include/exclude in this order (include first)?
   A.  Doesn't change correctness, only output order. Including
       first tends to find heavier subsets earlier, which can
       help when combined with bounding (more chances to
       trigger upper bound).

   ============================================================
   CROSS QUESTIONS
   ---------------
   CQ1. What if the array has negative numbers?
        The bounds break! upper-bound 'currSum+nums[idx] > target'
        assumes ascending order with only positive numbers. With
        negatives, even an overshoot might be 'fixed' by adding
        a negative later. You'd need different (looser) bounds.

   CQ2. Subset-sum vs 0/1 Knapsack?
        Subset-sum is the DECISION version (just feasibility).
        0/1 Knapsack is the OPTIMIZATION version. Subset-sum
        reduces to 0/1 Knapsack with value = weight: 'is there
        a subset with total = target?'

   CQ3. Pseudo-polynomial DP for subset-sum?
        dp[i][s] = true if first i elements can sum to s.
        dp[i][s] = dp[i-1][s] OR (s >= nums[i-1] AND
        dp[i-1][s - nums[i-1]]). O(n * target) time, O(target)
        space with 1D rolling array.

   CQ4. Merkle-Hellman cryptosystem was BROKEN. Why?
        Subset-sum is NP-Complete in general but the special
        SUPERINCREASING sequences used in Merkle-Hellman allow
        a polynomial-time attack (Shamir's algorithm). The
        general case remains hard.

   CQ5. Worst case of pruning?
        If target = totalSum / 2 and all elements are similar,
        very little pruning happens. The closer the target is
        to half the total, the harder the instance.
   ============================================================ */
