/* ============================================================
   TOPIC : MERGE SORT
   PARADIGM : Divide & Conquer
   ============================================================

   CONCEPT
   -------
   Split the array into halves recursively until each piece has
   one element (trivially sorted). Then merge the sorted halves
   back together. The actual sorting work happens during MERGE.
   Reliable O(n log n) on every input -- no surprises.

   BRUTE FORCE (for comparison)
   ----------------------------
   The brute-force way to sort n items is Bubble/Selection/
   Insertion Sort -- compare every pair, swap if out of order.
   These are all O(n^2). Merge Sort uses divide-and-conquer to
   bring this down to O(n log n) by never re-comparing elements
   already in the same sorted run.

   RECURRENCE RELATION
   -------------------
       T(n) = 2 T(n/2) + O(n)
   -- 2 recursive calls on halves, plus O(n) merge work.
   Master Theorem:  a = 2, b = 2, f(n) = n,  log_b(a) = 1.
       f(n) = n^1 = n^(log_b a)   =>  CASE 2
       => T(n) = O(n log n)

   COMPLEXITY
   ----------
       Best     : O(n log n)   (always splits + merges)
       Average  : O(n log n)
       Worst    : O(n log n)
       Space    : O(n)         auxiliary L[] and R[] arrays
       Stable   : YES          ( <= keeps left first on ties )
       In-place : NO           ( needs O(n) extra )

   REAL-WORLD USES
   ---------------
   * External sorting of files too big for RAM (100+ GB logs)
   * std::stable_sort uses a merge-sort variant
   * Linked-list sorting (sequential access fits merge)
   * Counting inversions in recommendation systems
   ============================================================ */

#include <iostream>
using namespace std;

/* --- merge two sorted halves arr[l..m] and arr[m+1..r] ----- */
void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;          // size of left half
    int n2 = r - m;              // size of right half
    int L[n1], R[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        // '<=' keeps merge sort STABLE (equal elements -> left first)
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else              arr[k++] = R[j++];
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

/* --- recursive divide step --------------------------------- */
void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        // 'l + (r-l)/2' avoids integer overflow that '(l+r)/2' would cause
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

int main() {
    int arr[] = {12, 11, 13, 5, 6, 7};
    int n = sizeof(arr) / sizeof(arr[0]);

    cout << "Input  : ";
    for (int i = 0; i < n; i++) cout << arr[i] << " ";
    cout << "\n";

    mergeSort(arr, 0, n - 1);

    cout << "Sorted : ";
    for (int i = 0; i < n; i++) cout << arr[i] << " ";
    cout << "\n";

    // edge-case demos
    int single[1] = {42};
    mergeSort(single, 0, 0);           // n=1 -> no-op (l < r is false)
    cout << "n=1 OK : " << single[0] << "\n";

    int empty_arr[1];                   // n=0 case
    mergeSort(empty_arr, 0, -1);       // l > r -> no-op, safe
    cout << "n=0 OK\n";
    return 0;
}

/* ============================================================
   LINE-BY-LINE EXPLANATION OF THE KEY PARTS
   ----------------------------------------
   1. mergeSort() is the divide step. It splits the index range
      until l == r (one element). Then unwinds the recursion,
      merging two sorted halves at every level.
   2. merge() is the conquer step. It walks two sorted halves
      with pointers i and j, picking the smaller front element.
   3. '<=' (not '<') makes the sort STABLE. With '<', equal
      elements from R would be placed before equal ones from L,
      breaking original order.
   4. 'm = l + (r-l)/2' is the overflow-safe midpoint formula.

   ============================================================
   VIVA QUESTIONS  (Q -> A)
   ------------------------
   Q1. What is the recurrence and how do you solve it?
   A.  T(n) = 2T(n/2) + O(n). Master Theorem case 2 (a=2,b=2,
       f(n)=n, log_b a = 1) gives O(n log n).

   Q2. Why is merge sort preferred over quick sort for linked
       lists?
   A.  Linked lists have O(n) random access -- that kills quick
       sort's partition step. Merge sort works sequentially with
       pointer rewiring, no random access needed. Plus merge
       sort is stable.

   Q3. Is merge sort stable? Why does it matter?
   A.  Yes. The merge uses '<=' so equal elements from L go
       first, preserving original order. Matters when sorting
       records by multiple keys (e.g. sort by age, then by name
       -- stability preserves the inner ordering).

   Q4. Why m = l + (r-l)/2 instead of (l+r)/2?
   A.  To avoid integer overflow when l and r are both near
       INT_MAX. The forms are mathematically equal but the
       subtraction-first version never overflows.

   Q5. Why is the space complexity O(n) and not O(n log n)?
   A.  At any moment only ONE pair of L[] and R[] arrays of the
       current level exists -- earlier-level arrays were freed
       when recursion unwound. Peak memory = top-level merge =
       O(n). Recursion stack adds O(log n) on top.

   ============================================================
   CROSS QUESTIONS
   ---------------
   CQ1. Can merge sort be done in-place?
        Yes, but it pushes complexity to O(n log^2 n) and the
        code becomes brittle. Standard version trades O(n) space
        for simplicity and stability.

   CQ2. Can you make merge sort iterative?
        Yes -- bottom-up merge sort. Start with subarrays of
        size 1, merge into size 2, then 4, 8 ... No recursion
        stack. Same O(n log n) time.

   CQ3. When is merge sort WORSE than quick sort in practice?
        On in-memory arrays of primitives: quick sort wins due
        to better cache locality and zero extra memory. Merge
        sort wins on linked lists, when stability is needed,
        and on data too big for RAM.

   CQ4. How do you count inversions using merge sort?
        During merge, whenever you pick from R before L is
        empty, every remaining element in L forms an inversion
        with the picked R element. Add (n1 - i) to the count.
        Total: O(n log n) inversion count.
   ============================================================ */
