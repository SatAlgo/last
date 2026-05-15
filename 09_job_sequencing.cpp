/* ============================================================
   TOPIC : JOB SEQUENCING WITH DEADLINES
   PARADIGM : Greedy
   ============================================================

   CONCEPT
   -------
   N jobs each with a deadline (in unit-time slots) and a
   profit. One job per unit time, one machine. Each job, if
   scheduled, must finish by its deadline. Maximize total
   profit.

   Greedy strategy: sort jobs by profit DESCENDING. For each
   job, schedule it in the LATEST free slot at or before its
   deadline. If no such slot exists, drop it.

   Why LATEST free slot? To preserve earlier slots for
   future, lower-profit jobs that have TIGHT (early) deadlines.

   BRUTE FORCE
   -----------
   Try every subset of jobs (2^n), check if any ordering meets
   all deadlines, take the max profit. O(2^n * n log n).

   COMPLEXITY (this implementation)
   --------------------------------
       Best     : O(n^2)       sort O(n log n) + scan O(n^2)
       Average  : O(n^2)
       Worst    : O(n^2)
       Space    : O(maxDeadline)
   With Disjoint Set Union (DSU) for finding next free slot,
   total drops to O(n log n).

   REAL-WORLD USES
   ---------------
   * CPU scheduling with priorities and deadlines (real-time OS)
   * Ad scheduling -- TV / online ads bid for time slots
   * Manufacturing job queues balancing deadlines and revenue
   * Hospital surgery scheduling with priority cases
   * Cloud function scheduling with SLA deadlines
   ============================================================ */

#include <iostream>
#include <algorithm>
#include <cstring>
using namespace std;

struct Job {
    char id;
    int deadline;
    int profit;
};

bool cmp(Job a, Job b) { return a.profit > b.profit; }

void jobSequencing(Job arr[], int n) {
    sort(arr, arr + n, cmp);

    int maxDeadline = 0;
    for (int i = 0; i < n; i++)
        if (arr[i].deadline > maxDeadline) maxDeadline = arr[i].deadline;

    /* slot[t] holds the id of job scheduled at time t (1..maxDeadline)
       '-' means empty.  We use a tiny stack-style array. */
    char slot[maxDeadline + 1];
    memset(slot, '-', sizeof(slot));

    int totalProfit = 0;
    int jobsDone = 0;

    for (int i = 0; i < n; i++) {
        /* try the LATEST possible slot for this job (closer to its
           deadline first), going backwards until we find a free slot. */
        for (int j = min(maxDeadline, arr[i].deadline); j > 0; j--) {
            if (slot[j] == '-') {
                slot[j] = arr[i].id;
                totalProfit += arr[i].profit;
                jobsDone++;
                break;
            }
        }
    }

    cout << "Scheduled job order: ";
    for (int i = 1; i <= maxDeadline; i++)
        if (slot[i] != '-') cout << slot[i] << " ";
    cout << "\n";
    cout << "Jobs done   : " << jobsDone << "\n";
    cout << "Total profit: " << totalProfit << "\n";
}

int main() {
    Job arr[] = {
        {'a', 2, 100},
        {'b', 1,  19},
        {'c', 2,  27},
        {'d', 1,  25},
        {'e', 3,  15}
    };
    int n = sizeof(arr) / sizeof(arr[0]);
    jobSequencing(arr, n);
    /* Expected schedule: c a e   total profit = 142 */
    return 0;
}

/* ============================================================
   LINE-BY-LINE EXPLANATION
   ------------------------
   cmp + sort: order jobs from highest profit to lowest.

   maxDeadline: any job past this can't be scheduled -- so we
   only need slots up to this value.

   slot[]: array of timeslots, initially all empty ('-').

   Outer loop: walk jobs in profit-descending order.
   Inner loop: scan slot[deadline] -> slot[deadline-1] -> ... 1
   for the FIRST free slot found going backwards. Place job there.

   WHY backwards? Because earlier slots are valuable -- they
   can accommodate jobs with TIGHTER deadlines. By preferring
   later slots first, we keep earlier slots open for future,
   urgent jobs.

   ============================================================
   VIVA QUESTIONS
   --------------
   Q1. Why pick the LATEST available slot, not the earliest?
   A.  To preserve flexibility. Early slots stay open for
       future, lower-profit jobs with tight (early) deadlines.
       If we used the earliest slot, a high-profit job with a
       late deadline could block jobs that NEED early slots.

   Q2. Can this be done faster than O(n^2)?
   A.  Yes -- using Disjoint Set Union (Union-Find) to find the
       next free slot in nearly O(1) -- total drops to
       O(n log n). Another approach uses a min-heap of accepted
       jobs.

   Q3. What if jobs have variable durations?
   A.  Then it becomes WEIGHTED INTERVAL SCHEDULING with
       variable durations -- NP-hard. You'd switch to DP.

   Q4. Proof of correctness?
   A.  Exchange argument. If an optimal schedule excludes the
       highest-profit job we picked, swapping it in (possibly
       displacing a lower-profit one) cannot decrease total
       profit. So our pick is part of some optimum.

   Q5. What's the difference between this and 'minimum number
       of machines' scheduling?
   A.  This is one machine, varying profits. Min-machines is
       'assign all jobs across as few machines as possible' --
       different objective, different algorithm (interval graph
       coloring).

   ============================================================
   CROSS QUESTIONS
   ---------------
   CQ1. What if two jobs have the same profit?
        Tie-breaking is arbitrary. You could prefer earlier
        deadline first, but it doesn't change optimality.

   CQ2. Could a different greedy (e.g. by deadline) work?
        Sorting by deadline alone misses profits. Sorting by
        profit/deadline ratio also fails in some cases. The
        profit-descending + latest-slot rule is provably
        optimal here.

   CQ3. How would DP look for variable-duration jobs?
        State: dp[t] = max profit using jobs whose deadlines
        are <= t. Transition: either skip job i or include it
        if its duration fits. Becomes weighted interval
        scheduling.

   CQ4. What's the relationship to GREEDY CHOICE PROPERTY?
        The argument above proves the greedy choice property
        holds: picking the highest-profit job and scheduling it
        in the latest slot is consistent with some optimal
        solution. Combined with optimal substructure (rest is
        smaller instance) -> greedy works.
   ============================================================ */
