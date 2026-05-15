/* ============================================================
   TOPIC : QUICK SORT  with  MEDIAN OF MEDIANS  (deterministic
           worst-case O(n log n))
   PARADIGM : Divide & Conquer
   ============================================================

   CONCEPT
   -------
   Median of Medians (MoM) is a deterministic pivot-selection
   scheme that guarantees the chosen pivot lies between the
   30th and 70th percentile of the array. Used as the pivot for
   quick sort, this gives worst-case O(n log n) -- no luck
   needed.

   How MoM picks the pivot:
     1. Divide the array into groups of 5.
     2. Sort each group, take its median.
     3. RECURSIVELY find the median of those n/5 medians.
     4. Use that as the pivot.

   BRUTE FORCE / NAIVE ALTERNATIVE
   -------------------------------
   Picking a random pivot gives EXPECTED O(n log n) but a
   small (vanishing) probability of O(n^2). MoM is a heavier
   but DETERMINISTIC guarantee -- valuable in safety-critical
   real-time systems.

   RECURRENCE RELATION  (for selection version of MoM)
   ---------------------------------------------------
       T(n) = T(n/5)        // recursive call for median-of-medians
            + T(7n/10)      // worst-case other side after partition
            + O(n)          // grouping & partition work
   Since 1/5 + 7/10 = 9/10 < 1, this solves to T(n) = O(n)
   for SELECTION. Used as pivot in QUICK SORT (which makes
   n log n decisions of size halved-by-at-least-30%), gives
   overall O(n log n) WORST case for sorting.

   COMPLEXITY
   ----------
       Best     : O(n log n)
       Average  : O(n log n)
       Worst    : O(n log n)   ** deterministic guarantee **
       Space    : O(log n)
       Stable   : NO
       In-place : YES (mostly -- some extra vectors)

   REAL-WORLD USES
   ---------------
   * Theoretical foundation for worst-case-bounded sorting
   * Linear-time SELECTION algorithm for k-th smallest
   * Used in critical systems where O(n^2) is unacceptable
   * Rarely used as PRIMARY sort in libraries -- constants too
     high; randomized quick sort wins in practice.
   ============================================================ */

#include <iostream>
#include <algorithm>
#include <vector>
#include <climits>
using namespace std;

/* --- median of a sub-array of up to 5 elements ------------- */
int findMedian(vector<int>& arr, int l, int n) {
    sort(arr.begin() + l, arr.begin() + l + n);
    return arr[l + n / 2];
}

/* --- partition arr[l..r] around the given pivot value x ---- */
int partitionAround(vector<int>& arr, int l, int r, int x) {
    int i;
    for (i = l; i < r; i++) if (arr[i] == x) break;
    swap(arr[i], arr[r]);                   // move pivot to end
    i = l;
    for (int j = l; j < r; j++)
        if (arr[j] <= x) { swap(arr[i], arr[j]); i++; }
    swap(arr[i], arr[r]);
    return i;
}

/* --- k-th smallest in arr[l..r] (1-indexed k) -------------- */
int kthSmallest(vector<int>& arr, int l, int r, int k) {
    if (k > 0 && k <= r - l + 1) {
        int n = r - l + 1;
        vector<int> medians((n + 4) / 5);
        int i;
        for (i = 0; i < n / 5; i++)
            medians[i] = findMedian(arr, l + i * 5, 5);
        if (i * 5 < n) {
            medians[i] = findMedian(arr, l + i * 5, n % 5);
            i++;
        }
        int medOfMed = (i == 1) ? medians[0]
                                : kthSmallest(medians, 0, i - 1, i / 2);
        int pos = partitionAround(arr, l, r, medOfMed);
        if (pos - l == k - 1) return arr[pos];
        if (pos - l > k - 1)  return kthSmallest(arr, l, pos - 1, k);
        return kthSmallest(arr, pos + 1, r, k - pos + l - 1);
    }
    return INT_MAX;
}

/* --- quick sort that uses MoM for pivot -------------------- */
void quickSortMoM(vector<int>& arr, int l, int r) {
    if (l < r) {
        int n = r - l + 1;
        int median = kthSmallest(arr, l, r, n / 2 + 1);
        int pi = partitionAround(arr, l, r, median);
        quickSortMoM(arr, l, pi - 1);
        quickSortMoM(arr, pi + 1, r);
    }
}

int main() {
    vector<int> arr = {12, 3, 5, 7, 4, 19, 26, 21, 8};

    cout << "Input  : ";
    for (int x : arr) cout << x << " ";
    cout << "\n";

    // demo: find 3rd smallest in a copy first
    vector<int> copy_arr = arr;
    cout << "3rd smallest = "
         << kthSmallest(copy_arr, 0, copy_arr.size() - 1, 3) << "\n";

    quickSortMoM(arr, 0, arr.size() - 1);

    cout << "Sorted : ";
    for (int x : arr) cout << x << " ";
    cout << "\n";
    return 0;
}

/* ============================================================
   LINE-BY-LINE EXPLANATION
   ------------------------
   findMedian(): sorts a tiny (<=5) chunk and returns its
   middle element. Sorting a constant-size array is O(1).

   kthSmallest(): the workhorse.
     1. Splits arr[l..r] into chunks of 5, finds median of each.
     2. Stores medians in a vector 'medians'.
     3. Recursively picks the MEDIAN of those medians as pivot.
     4. Partitions around it; recurses into the side containing
        the k-th position.

   quickSortMoM(): plain quick sort but uses kthSmallest with
   k = n/2 + 1 to pick the EXACT MEDIAN as pivot, guaranteeing
   balanced partitions.

   ============================================================
   VIVA QUESTIONS
   --------------
   Q1. Why does median-of-medians guarantee O(n log n)?
   A.  The chosen pivot is guaranteed to lie between the 30th
       and 70th percentile of the array. So neither partition
       side exceeds 70% of n. Recursion depth stays logarithmic.

   Q2. Why groups of 5 specifically -- not 3, not 7?
   A.  Groups of 3 give T(n) = T(n/3) + T(2n/3) + O(n) which is
       O(n log n) -- NOT linear, so MoM-selection wouldn't be
       linear, breaking the proof. Groups of 5 give T(n) =
       T(n/5) + T(7n/10) + O(n), and 1/5 + 7/10 = 9/10 < 1
       which solves to O(n). 5 is the smallest odd number that
       works.

   Q3. Why must the group size be ODD?
   A.  So each group has a unique median. With even size you'd
       have to pick one of two middle elements, and the
       guarantee becomes weaker.

   Q4. Is this used in practice?
   A.  Rarely as a primary sort. Randomized quick sort is
       simpler and faster in practice (lower constants). The
       SELECTION variant (k-th smallest in linear time worst-
       case) is more commonly seen.

   Q5. Recurrence for the SELECTION version of MoM?
   A.  T(n) <= T(n/5) + T(7n/10) + O(n).  Since 1/5 + 7/10 < 1
       it solves to O(n) -- linear-time selection.

   ============================================================
   CROSS QUESTIONS
   ---------------
   CQ1. Why exactly 30%-70% guarantee?
        At least half the medians are >= MoM. Each of those
        groups contributes at least 3 elements that are >= MoM.
        That's >= 3*(n/5)/2 = 3n/10 elements. So at least 30%
        are >= MoM and at least 30% are <= MoM -- pivot is in
        the middle 40%.

   CQ2. Can we do worst-case selection in linear time without
        MoM?
        BFPRT algorithm (Blum-Floyd-Pratt-Rivest-Tarjan) IS
        the median-of-medians approach. There's no
        substantively different deterministic linear selection
        algorithm.

   CQ3. Difference between PARTITIONING and SORTING here?
        Partitioning around a value runs once and is O(n).
        Sorting calls partition O(log n) times (balanced
        because pivot is good). MoM ensures the log n bound is
        deterministic.

   CQ4. Can we improve constants with a tuned MoM?
        Yes -- ninther (median of 3 medians of 3) used in
        practice in some sort implementations. Doesn't change
        asymptotics but removes most worst-case scenarios.
   ============================================================ */
