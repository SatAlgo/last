/* ============================================================
   TOPIC : FRACTIONAL KNAPSACK
   PARADIGM : Greedy
   ============================================================

   CONCEPT
   -------
   Maximize value in a knapsack of capacity W when items are
   DIVISIBLE (you can take any fraction of an item). Strategy:
   sort items by value/weight ratio in descending order, then
   greedily take items until full -- taking a FRACTION of the
   last item if it doesn't fully fit.

   This works because the per-unit value never improves -- once
   we've picked the item with the highest ratio, the next best
   per-unit choice is the second-highest ratio, and so on.

   BRUTE FORCE
   -----------
   Try every fraction split. There are infinitely many real-
   valued fractions, so brute force isn't even well-defined
   here -- which is exactly why the greedy choice is special.
   (For the 0/1 INDIVISIBLE version, brute force is 2^n.)

   COMPLEXITY
   ----------
       Best     : O(n log n)   sorting dominates
       Average  : O(n log n)
       Worst    : O(n log n)
       Space    : O(1)         in-place sort, O(n) if you keep
                                items separately

   PROOF OF CORRECTNESS (EXCHANGE ARGUMENT)
   ----------------------------------------
   Suppose optimal doesn't take as much as possible of the
   highest-ratio item. Swap some lower-ratio fraction with
   higher-ratio fraction (same weight). Total value strictly
   increases or stays the same -- contradicting 'optimal'. So
   the greedy choice is part of some optimal solution.

   REAL-WORLD USES
   ---------------
   * Bandwidth allocation between traffic classes
   * Cargo loading for liquids / grains (literally divisible)
   * Investment portfolio when fractional shares allowed
   * Cloud resource allocation (CPU / RAM divisible)
   * Mixing ingredients to maximize nutritional value within
     a calorie budget
   ============================================================ */

#include <iostream>
#include <algorithm>
using namespace std;

struct Item {
    int value;
    int weight;
};

/* sort by value/weight ratio DESCENDING (highest first) */
bool cmp(Item a, Item b) {
    return (double)a.value / a.weight > (double)b.value / b.weight;
}

double fractionalKnapsack(int W, Item arr[], int n) {
    sort(arr, arr + n, cmp);

    double totalValue = 0.0;
    for (int i = 0; i < n; i++) {
        if (arr[i].weight <= W) {
            /* take whole item */
            W -= arr[i].weight;
            totalValue += arr[i].value;
            cout << "Took ALL of item with value=" << arr[i].value
                 << ", weight=" << arr[i].weight << "\n";
        } else {
            /* take a fraction of this item then stop */
            double frac = (double)W / arr[i].weight;
            totalValue += arr[i].value * frac;
            cout << "Took FRACTION " << frac
                 << " of item with value=" << arr[i].value
                 << ", weight=" << arr[i].weight << "\n";
            break;
        }
    }
    return totalValue;
}

int main() {
    Item arr[] = {{60, 10}, {100, 20}, {120, 30}};
    int W = 50;
    int n = sizeof(arr) / sizeof(arr[0]);

    cout << "Capacity = " << W << "\n";
    double maxValue = fractionalKnapsack(W, arr, n);
    cout << "Maximum value = " << maxValue << "\n";

    // edge cases
    Item single[] = {{42, 10}};
    cout << "\nW=5, single item value 42 weight 10 -> "
         << fractionalKnapsack(5, single, 1) << " (should be 21)\n";

    Item huge[] = {{50, 5}};
    cout << "W=100 but item too small (5,50) -> "
         << fractionalKnapsack(100, huge, 1) << " (should be 50)\n";
    return 0;
}

/* ============================================================
   LINE-BY-LINE EXPLANATION
   ------------------------
   cmp(): comparator returns true if a's ratio > b's, so sort()
   places higher-ratio items earlier.

   fractionalKnapsack():
     1. Sort items by ratio descending.
     2. Walk sorted items. For each:
        - If full item fits, take it whole. Subtract its weight
          from W, add its full value.
        - Otherwise take only the fraction W/weight, add the
          proportional value, then break (knapsack is full).

   ============================================================
   VIVA QUESTIONS
   --------------
   Q1. Why does greedy work for fractional but not 0/1
       knapsack?
   A.  Because items are DIVISIBLE. After taking the highest-
       ratio item completely, the next best per-unit choice
       remains optimal -- no 'wasted' capacity issue. In 0/1
       you might pick a high-ratio item that leaves capacity
       you can't fill, losing more than you gained.

   Q2. Prove fractional knapsack greedy is optimal.
   A.  Exchange argument. Suppose optimal solution doesn't fully
       use the highest-ratio item. Swap some lower-ratio
       fraction with higher-ratio fraction of equal weight.
       Total value can only INCREASE -- contradicting
       'optimal'. So greedy belongs to some optimal solution.

   Q3. What if all items have the same value/weight ratio?
   A.  Any order works. Total value depends only on total
       weight taken. Greedy still produces optimal.

   Q4. What if you must take whole items?
   A.  That's the 0/1 knapsack -- NP-complete. Greedy fails;
       use DP for pseudo-polynomial O(nW), or branch-and-bound
       for sometimes-faster real-world inputs.

   Q5. Complexity bottleneck?
   A.  Sorting: O(n log n). The greedy traversal is O(n). So
       the total is O(n log n) dominated by sort.

   ============================================================
   CROSS QUESTIONS
   ---------------
   CQ1. Can fractional knapsack be solved without sorting?
        Using QuickSelect for partition around the median ratio
        repeatedly, you can solve it in O(n) -- linear time.
        Not commonly taught but theoretically interesting.

   CQ2. What if items have negative values?
        Just skip them -- never include items with v <= 0 in a
        maximization problem.

   CQ3. Difference between greedy choice property and optimal
        substructure?
        Greedy choice property: a globally optimal solution can
        be reached by making locally optimal choices.
        Optimal substructure: optimal solution contains optimal
        solutions to subproblems. BOTH are required for greedy
        to work. Fractional knapsack has both; 0/1 has only
        optimal substructure -> DP.

   CQ4. How would you handle 'bounded' knapsack (each item has
        a count limit)?
        Either DP (treat each item as a separate copy up to its
        count, classic bounded knapsack) or smart-binary
        decomposition (split count into powers of 2).
   ============================================================ */
