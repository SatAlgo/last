/* ============================================================
   TOPIC : RANDOMIZED QUICK SORT
   PARADIGM : Divide & Conquer + Randomized
   ============================================================

   CONCEPT
   -------
   Standard quick sort but the PIVOT is chosen UNIFORMLY AT
   RANDOM from the current sub-array. This eliminates worst-case
   O(n^2) on adversarial (already-sorted / reverse-sorted)
   inputs because no input pattern can force bad pivots --
   pivot quality is now a random variable independent of input.

   BRUTE FORCE / NAIVE QUICK SORT
   ------------------------------
   Picking arr[high] as pivot:  if input is already sorted,
   every partition is maximally unbalanced -> O(n^2).
   Randomization is the CHEAP fix.

   RECURRENCE RELATION
   -------------------
   E[T(n)] = sum_{i=0..n-1} (1/n) * ( T(i) + T(n-1-i) ) + O(n)
   This solves to E[T(n)] = O(n log n).
   Worst case (unlucky pivots every time) still O(n^2) but its
   probability is roughly (1/n)^n -- astronomical.

   COMPLEXITY
   ----------
       Best     : O(n log n)
       Average  : O(n log n)   EXPECTED, holds for ALL inputs
       Worst    : O(n^2)        but probability -> 0 fast
       Space    : O(log n)     expected recursion depth
       Stable   : NO
       In-place : YES

   REAL-WORLD USES
   ---------------
   * Sorting in security-sensitive systems where adversarial
     inputs are a worry (a crafted input can DoS a deterministic
     quick sort)
   * QuickSelect (k-th smallest in expected O(n)) uses random
     pivots
   * Load balancing -- pick a random server, avoid hot spots

   LAS VEGAS vs MONTE CARLO
   ------------------------
   This is a LAS VEGAS algorithm: the answer is ALWAYS correct,
   only RUNNING TIME is random. Monte Carlo would risk a wrong
   answer (e.g. Miller-Rabin primality).
   ============================================================ */

#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return i + 1;
}

/* --- pick random pivot, swap it to 'high', then partition --- */
int randomPartition(int arr[], int low, int high) {
    int n = high - low + 1;
    int random = low + rand() % n;      // uniform pick in [low, high]
    swap(arr[random], arr[high]);
    return partition(arr, low, high);
}

void randomizedQuickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = randomPartition(arr, low, high);
        randomizedQuickSort(arr, low, pi - 1);
        randomizedQuickSort(arr, pi + 1, high);
    }
}

int main() {
    srand(time(0));                     // seed once at startup

    int arr[] = {10, 7, 8, 9, 1, 5};
    int n = sizeof(arr) / sizeof(arr[0]);

    cout << "Input  : ";
    for (int i = 0; i < n; i++) cout << arr[i] << " ";
    cout << "\n";

    randomizedQuickSort(arr, 0, n - 1);

    cout << "Sorted : ";
    for (int i = 0; i < n; i++) cout << arr[i] << " ";
    cout << "\n";

    // KEY DEMO: already-sorted input that would BREAK deterministic
    // last-element-pivot quick sort now works in O(n log n) expected.
    int sorted_in[] = {1, 2, 3, 4, 5, 6, 7, 8};
    randomizedQuickSort(sorted_in, 0, 7);
    cout << "Already-sorted input handled : ";
    for (int x : sorted_in) cout << x << " ";
    cout << "\n";
    return 0;
}

/* ============================================================
   LINE-BY-LINE EXPLANATION
   ------------------------
   randomPartition():
     1. n = high - low + 1 is the current sub-array size.
     2. 'low + rand() % n' picks a uniformly random index in
        [low, high].
     3. We SWAP that random element with arr[high] so the
        existing Lomuto partition (which expects pivot at high)
        works unchanged.
     4. Then we call the normal partition.

   srand(time(0)) in main() seeds the PRNG. Without it, rand()
   returns the SAME sequence every run, defeating the point.

   ============================================================
   VIVA QUESTIONS
   --------------
   Q1. Why is randomized quick sort 'expected O(n log n)' and
       not just 'O(n log n)'?
   A.  Because the actual runtime depends on random choices.
       The EXPECTATION is O(n log n) but a single run might be
       lucky or unlucky. Over many runs the average is
       O(n log n).

   Q2. Does randomization eliminate the worst case completely?
   A.  No -- the worst case still EXISTS (unlikely pivots every
       time). What randomization eliminates is the WORST-CASE
       INPUT: no input pattern can force bad pivots.

   Q3. Las Vegas vs Monte Carlo -- which is this?
   A.  Las Vegas. Output is always correct (truly sorted); only
       runtime is a random variable. Monte Carlo would have a
       chance of wrong output but bounded runtime (e.g. Miller-
       Rabin primality might say PROBABLY prime).

   Q4. Why does this beat deterministic median-of-three?
   A.  Median-of-three can still be defeated by a crafted
       adversarial input. Random pivots can't be predicted by
       any input pattern -- the input cannot 'see' the random
       choice. This is algorithmic SECURITY, not just speed.

   Q5. What's the recurrence and why does it solve to nlogn?
   A.  E[T(n)] = (1/n) * SUM(T(i) + T(n-1-i)) + O(n).
       Each partition split is uniformly distributed. Solving
       gives E[T(n)] = O(n log n) -- the expected partition
       sizes are balanced ON AVERAGE.

   ============================================================
   CROSS QUESTIONS
   ---------------
   CQ1. How would you derandomize this into a deterministic
        O(n log n) algorithm?
        Use Median of Medians as pivot. Guaranteed pivot lies
        between 30th and 70th percentile -> partition never
        exceeds 70% of n -> deterministic O(n log n). But
        higher constants make it slower in practice.

   CQ2. What's the probability of hitting O(n^2)?
        Roughly (1/n)^n -- need every single pivot to be near
        the extremes. For n = 1000 that's < 10^(-3000). Won't
        happen in the universe's lifetime.

   CQ3. Is rand() in C++ a 'good enough' random source?
        For sorting -- yes. For cryptography -- no. C++11
        introduced <random> with much better PRNGs
        (mt19937 + uniform_int_distribution).

   CQ4. Randomized quick SORT vs randomized quick SELECT?
        Same idea, different goal. Quick SELECT finds k-th
        smallest -- after partition, recurse only into the half
        containing index k. Expected O(n) total, not O(n log n).
   ============================================================ */
