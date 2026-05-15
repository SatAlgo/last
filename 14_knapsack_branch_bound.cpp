/* ============================================================
   TOPIC : 0/1 KNAPSACK  (Branch & Bound)
   PARADIGM : Branch & Bound
   ============================================================

   CONCEPT
   -------
   Build a state-space tree where each node decides
   INCLUDE / EXCLUDE for one item. For each node compute an
   UPPER BOUND on the best value still reachable from it (using
   fractional-knapsack relaxation). If the bound <= current
   best (maxProfit), PRUNE the branch.

   Use a PRIORITY QUEUE (best-first search) keyed on the upper
   bound -- explore the most-promising node first so a good
   maxProfit is found early, enabling more aggressive pruning.

   BRUTE FORCE
   -----------
   Enumerate all 2^n subsets, evaluate each. O(2^n). B&B uses
   bounds to skip whole subtrees, often FAR fewer in practice.

   COMPLEXITY
   ----------
       Best     : O(n)         lucky pruning
       Average  : varies a lot, often much less than 2^n
       Worst    : O(2^n)       no pruning, same as brute force
       Space    : O(2^n)        priority queue can grow

   REAL-WORLD USES
   ---------------
   * Travelling Salesman solvers use same B&B framework
   * Integer Linear Programming solvers (CPLEX, Gurobi)
   * VLSI chip design (placing components optimally)
   * Job-shop scheduling in factories
   * Crew assignment in airlines

   B&B vs BACKTRACKING
   -------------------
   Backtracking finds ANY feasible solution (constraint
   satisfaction); it prunes when CONSTRAINTS are violated.
   B&B is for OPTIMIZATION; it prunes when BOUNDS show a
   branch cannot improve the best so far.
   ============================================================ */

#include <iostream>
#include <queue>
#include <algorithm>
using namespace std;

struct Item { float value, weight; };

struct Node {
    int   level;     // index of the item being decided
    float profit;    // accumulated profit
    float weight;    // accumulated weight
    float bound;     // upper bound on subtree
};

bool cmp(Item a, Item b) {
    return (a.value / a.weight) > (b.value / b.weight);
}

/* upper bound = current profit + greedy fractional knapsack on remaining */
float bound(Node u, int n, int W, Item arr[]) {
    if (u.weight >= W) return 0;
    float profit_bound = u.profit;
    int j = u.level + 1;
    float totweight = u.weight;
    while (j < n && totweight + arr[j].weight <= W) {
        totweight   += arr[j].weight;
        profit_bound += arr[j].value;
        j++;
    }
    if (j < n)
        profit_bound += (W - totweight) * arr[j].value / arr[j].weight;
    return profit_bound;
}

struct Compare {
    bool operator()(Node a, Node b) { return a.bound < b.bound; }
};

float knapsackBB(int W, Item arr[], int n) {
    sort(arr, arr + n, cmp);                   // sort by ratio descending

    priority_queue<Node, vector<Node>, Compare> pq;
    Node u, v;
    u.level = -1; u.profit = 0; u.weight = 0;
    u.bound = bound(u, n, W, arr);
    pq.push(u);

    float maxProfit = 0;
    while (!pq.empty()) {
        u = pq.top(); pq.pop();
        if (u.bound <= maxProfit) continue;    // prune
        if (u.level == n - 1)     continue;

        /* LEFT child: take item u.level+1 */
        v.level  = u.level + 1;
        v.weight = u.weight + arr[v.level].weight;
        v.profit = u.profit + arr[v.level].value;
        if (v.weight <= W && v.profit > maxProfit) maxProfit = v.profit;
        v.bound = bound(v, n, W, arr);
        if (v.bound > maxProfit) pq.push(v);

        /* RIGHT child: skip item u.level+1 */
        v.weight = u.weight;
        v.profit = u.profit;
        v.bound  = bound(v, n, W, arr);
        if (v.bound > maxProfit) pq.push(v);
    }
    return maxProfit;
}

int main() {
    Item arr[] = {{40, 2}, {30, 5}, {50, 10}, {10, 5}};
    int n = sizeof(arr) / sizeof(arr[0]);
    int W = 16;

    cout << "Capacity = " << W << "\n";
    cout << "Items (value,weight): ";
    for (int i = 0; i < n; i++)
        cout << "(" << arr[i].value << "," << arr[i].weight << ") ";
    cout << "\n";

    cout << "Max profit (B&B): " << knapsackBB(W, arr, n) << "\n";
    /* Expected output 90  (items value 40+50=90, weight 2+10=12 fits in 16) */
    return 0;
}

/* ============================================================
   LINE-BY-LINE EXPLANATION
   ------------------------
   Sort by ratio descending: bound() relies on this ordering --
   it greedily fills remaining capacity in highest-ratio order.

   bound(): computes the FRACTIONAL knapsack value from the
   current node downward. Since fractional >= 0/1 always, this
   is a valid upper bound on the 0/1 optimum reachable below
   this node.

   Priority queue: max-heap on bound (Compare returns true when
   a.bound < b.bound -> b sits on top). This is BEST-FIRST
   search: explore the most promising node first to find a
   good maxProfit early, enabling pruning.

   Two children per pop: include the next item (left) and skip
   it (right). For each, compute bound; only push if bound
   exceeds current maxProfit (otherwise it can't improve --
   pruned).

   ============================================================
   VIVA QUESTIONS
   --------------
   Q1. Difference between Backtracking and Branch & Bound?
   A.  Backtracking finds ANY feasible solution (constraint
       satisfaction). Prunes when CONSTRAINTS are violated.
       B&B is for OPTIMIZATION. Prunes when BOUNDS show a
       branch can't improve the current best.

   Q2. Why use a priority queue (best-first) instead of a stack
       (depth-first)?
   A.  Best-first explores the most promising node first,
       finding good solutions early. That raises the maxProfit
       threshold, enabling MORE aggressive pruning of remaining
       branches. DFS would explore in arbitrary order and miss
       early pruning opportunities.

   Q3. What is the bound function based on?
   A.  Fractional knapsack relaxation. If we relaxed the 0/1
       constraint to allow fractions, what's the maximum?
       That value is always >= 0/1 optimum -- so it's a valid
       UPPER BOUND on what this subtree can achieve.

   Q4. When does B&B perform NO BETTER than brute force?
   A.  When the bound function is loose (close to infinity, or
       always equals current profit + something huge) so no
       pruning happens. Quality of bound = quality of pruning.

   Q5. Why include and exclude as TWO children?
   A.  Standard binary state-space tree. Every subset
       corresponds to exactly one root-to-leaf path. Together
       they cover all 2^n subsets but pruning skips most.

   ============================================================
   CROSS QUESTIONS
   ---------------
   CQ1. B&B vs DP for 0/1 knapsack?
        DP: O(n*W) deterministic. Bad if W is enormous.
        B&B: variable runtime, can be much faster when bounds
        are tight, can be 2^n in the worst case. Best in
        practice for big W with moderate n.

   CQ2. Could you tighten the bound further?
        Yes -- use LP relaxation that includes more constraints
        (e.g., second-best fractional value as well). Tighter
        bound -> more pruning but more compute per node.
        Trade-off.

   CQ3. Why does sorting by RATIO matter?
        Because the fractional bound takes the highest-ratio
        items first. Without sorting, the bound is loose and
        much less useful for pruning.

   CQ4. Memory complexity of B&B?
        The priority queue can grow to O(2^n) in the worst case
        -- one node per possible subset. That's a real concern
        for large n.

   CQ5. Why is the worst case still O(2^n)?
        If bounds are tight everywhere (e.g., all items have
        identical ratios), no pruning occurs -- the algorithm
        explores the full tree, identical to brute force.
   ============================================================ */
