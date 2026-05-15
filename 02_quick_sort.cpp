/* ============================================================
   TOPIC : QUICK SORT  (basic)
   PARADIGM : Divide & Conquer
   ============================================================

   CONCEPT
   -------
   Pick a pivot (here: last element). PARTITION the array so
   elements <= pivot land left, > pivot land right. Then
   recursively sort each side. In-place and cache-friendly --
   usually the FASTEST sort in practice on random data.

   BRUTE FORCE
   -----------
   Same as merge sort's brute force baseline -- O(n^2) bubble /
   selection / insertion sort. Quick sort beats them by
   partitioning around a pivot so two halves are sorted
   INDEPENDENTLY -- the pivot's final position is never re-
   touched.

   RECURRENCE RELATION
   -------------------
   Best  / Average :  T(n) = 2 T(n/2) + O(n)  ->  O(n log n)
   Worst           :  T(n) = T(n-1)  + O(n)  ->  O(n^2)
   The worst recurrence sums to n + (n-1) + ... + 1 = O(n^2).

   COMPLEXITY
   ----------
       Best     : O(n log n)   pivot always near median
       Average  : O(n log n)   on random input
       Worst    : O(n^2)       pivot always min or max
       Space    : O(log n)     recursion stack (best/avg)
                  O(n)         recursion stack (worst)
       Stable   : NO           partition swaps cross long gaps
       In-place : YES          O(1) extra besides recursion

   REAL-WORLD USES
   ---------------
   * std::sort uses Introsort = quick sort + heap sort fallback
   * Database query engines: cache-friendly in-memory sorting
   * Java's Arrays.sort uses dual-pivot quick sort
   * Most language standard libraries pick quick-sort-family
   ============================================================ */

#include <iostream>
using namespace std;

/* --- Lomuto partition: pivot = arr[high] ------------------- */
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;                // i tracks end of '<= pivot' zone
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);    // place pivot in its final spot
    return i + 1;                    // pivot's final index
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);    // left of pivot
        quickSort(arr, pi + 1, high);   // right of pivot
    }
}

int main() {
    int arr[] = {10, 7, 8, 9, 1, 5};
    int n = sizeof(arr) / sizeof(arr[0]);

    cout << "Input  : ";
    for (int i = 0; i < n; i++) cout << arr[i] << " ";
    cout << "\n";

    quickSort(arr, 0, n - 1);

    cout << "Sorted : ";
    for (int i = 0; i < n; i++) cout << arr[i] << " ";
    cout << "\n";

    // EDGE CASE: already sorted -> hits WORST CASE pivot pattern
    int already[] = {1, 2, 3, 4, 5};
    quickSort(already, 0, 4);
    cout << "Already sorted still works : ";
    for (int x : already) cout << x << " ";
    cout << "\n";

    // EDGE CASE: duplicates
    int dups[] = {3, 1, 3, 2, 3, 1};
    quickSort(dups, 0, 5);
    cout << "With duplicates : ";
    for (int x : dups) cout << x << " ";
    cout << "\n";
    return 0;
}

/* ============================================================
   LINE-BY-LINE EXPLANATION
   ------------------------
   partition():
     'i' is the boundary -- everything at indices [low..i] is
     known to be <= pivot.  We scan j from low to high-1; on
     finding arr[j] <= pivot we EXTEND the zone by incrementing
     i and swapping arr[i] with arr[j]. After the loop we swap
     arr[i+1] (first element > pivot) with the pivot itself.

   quickSort():
     Sorts left of pi independently of right of pi -- pivot is
     already in its final position so we never touch it again.

   ============================================================
   VIVA QUESTIONS
   --------------
   Q1. When does quick sort hit O(n^2)?
   A.  When pivot is consistently the smallest or largest.
       Example: pivot = last element on an already-sorted array
       -> each partition gives one empty side and one of n-1
       -> n levels of O(n) work -> O(n^2).

   Q2. Quick sort vs Merge sort -- which is faster in practice?
   A.  Quick sort, usually. It's in-place (no O(n) extra) and
       cache-friendly (sequential memory access). Merge sort is
       safer (guaranteed O(n log n)) and stable.

   Q3. Is quick sort stable?
   A.  NO. The partition swap moves elements across long
       distances, so equal elements can change relative order.

   Q4. How to avoid the worst case?
   A.  Three options: (1) RANDOMIZED pivot, (2) Median-of-three
       (pick median of first/middle/last), (3) Median-of-
       Medians for a deterministic O(n log n) guarantee.

   Q5. Why is space complexity O(log n) and not O(1)?
   A.  Because of the recursion stack. Each recursive call
       pushes a stack frame. With balanced partitions, depth is
       log n. Worst case (unbalanced) goes to O(n).

   ============================================================
   CROSS QUESTIONS
   ---------------
   CQ1. What is recursion depth in worst case?
        O(n). That can cause stack overflow on huge inputs.
        std::sort = Introsort precisely to defend against this:
        if depth exceeds ~2 log n, it switches to heap sort.

   CQ2. Lomuto vs Hoare partition?
        Lomuto (this code): simple, more swaps. Hoare: uses two
        pointers from both ends, fewer swaps on average, but
        the pivot's final position is harder to track and
        partition can split unequally.

   CQ3. Tail-call optimization for quick sort?
        After partition, recurse on the SMALLER side and turn
        the larger side into iteration (loop). Caps stack depth
        at O(log n) even in worst case.

   CQ4. Why does quick sort dominate in practice even with
        O(n^2) worst case?
        Cache locality. Quick sort accesses memory sequentially
        from both ends in partition -- prefetcher loves it.
        Merge sort's L[] and R[] arrays cause two extra scans.
   ============================================================ */
