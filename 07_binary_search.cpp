/* ============================================================
   TOPIC : BINARY SEARCH
   PARADIGM : Divide & Conquer
   ============================================================

   CONCEPT
   -------
   Search a SORTED array for a target value by repeatedly
   halving the search range. Compare target with the middle
   element; if equal, done. If target is smaller, search left
   half; else search right half. Each step eliminates half the
   remaining candidates -> O(log n).

   BRUTE FORCE
   -----------
   Linear search: scan all n elements in order, O(n). Binary
   search exploits the SORTED property to skip half the array
   each step.

   RECURRENCE RELATION
   -------------------
       T(n) = T(n/2) + O(1)
   Master Theorem: a = 1, b = 2, f(n) = 1.
       log_b a = 0,  f(n) = n^0 = 1   =>  CASE 2
       =>  T(n) = O(log n)

   COMPLEXITY
   ----------
       Best     : O(1)        target is at the middle on first try
       Average  : O(log n)
       Worst    : O(log n)    target absent or at extreme
       Space    : O(1)        iterative
                  O(log n)    recursive (stack frames)

   REAL-WORLD USES
   ---------------
   * std::lower_bound / std::upper_bound / std::binary_search
   * Database B+Tree index lookups
   * Dictionary / phonebook lookup
   * Version-control 'git bisect' to find the commit that
     introduced a bug
   * Finding floor/ceiling values in sorted data
   * Used as a SUBROUTINE inside many other algorithms
   ============================================================ */

#include <iostream>
#include <vector>
using namespace std;

/* --- iterative version (preferred) ------------------------- */
int binarySearchIter(vector<int>& arr, int target) {
    int low = 0, high = arr.size() - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;   // overflow-safe midpoint
        if (arr[mid] == target) return mid;
        if (arr[mid] <  target) low  = mid + 1;
        else                    high = mid - 1;
    }
    return -1;
}

/* --- recursive version ------------------------------------- */
int binarySearchRec(vector<int>& arr, int low, int high, int target) {
    if (low > high) return -1;
    int mid = low + (high - low) / 2;
    if (arr[mid] == target) return mid;
    if (arr[mid] <  target) return binarySearchRec(arr, mid + 1, high, target);
    return                         binarySearchRec(arr, low, mid - 1, target);
}

/* --- ADVANCED: find first occurrence (lower_bound-style) ---- */
int firstOccurrence(vector<int>& arr, int target) {
    int low = 0, high = arr.size() - 1, result = -1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] == target) {
            result = mid;       // remember and KEEP searching LEFT
            high   = mid - 1;
        } else if (arr[mid] < target) low = mid + 1;
        else                          high = mid - 1;
    }
    return result;
}

int main() {
    vector<int> arr = {2, 5, 8, 12, 16, 23, 38, 56, 72, 91};

    cout << "Iterative search for 23 : index "
         << binarySearchIter(arr, 23) << "\n";

    cout << "Recursive search for 91 : index "
         << binarySearchRec(arr, 0, arr.size() - 1, 91) << "\n";

    cout << "Search for 100 (absent) : "
         << binarySearchIter(arr, 100) << "\n";

    // first occurrence in duplicates
    vector<int> dups = {1, 2, 2, 2, 3, 4, 5};
    cout << "First occurrence of 2 in [1,2,2,2,3,4,5] : "
         << firstOccurrence(dups, 2) << "\n";

    // edge cases
    vector<int> empty_arr;
    cout << "Empty array search : " << binarySearchIter(empty_arr, 5) << "\n";

    vector<int> one = {42};
    cout << "Single-element match    : " << binarySearchIter(one, 42) << "\n";
    cout << "Single-element no-match : " << binarySearchIter(one, 99) << "\n";
    return 0;
}

/* ============================================================
   LINE-BY-LINE EXPLANATION
   ------------------------
   Iterative:
     Two pointers: low (inclusive) and high (inclusive). Loop
     while there's a candidate range (low <= high). Compute
     mid using overflow-safe formula. Three branches: equal,
     less, greater.

   Recursive:
     Same idea, but recursion replaces the loop. Costs O(log n)
     stack memory.

   First occurrence:
     Standard binary search returns ANY match. To find the
     leftmost, when arr[mid] == target we don't stop -- we
     remember mid and KEEP searching the LEFT half. The 'result'
     variable accumulates the leftmost match seen.

   ============================================================
   VIVA QUESTIONS
   --------------
   Q1. Why must the input be sorted?
   A.  Because we use the sorted property to decide which half
       contains the target. Without it, eliminating a half is
       impossible -- linear search is the only option.

   Q2. Why low + (high - low) / 2 instead of (low + high) / 2?
   A.  To avoid INTEGER OVERFLOW when low and high are both
       near INT_MAX. The subtraction version is mathematically
       equal but never overflows.

   Q3. Recurrence and Master Theorem solution?
   A.  T(n) = T(n/2) + O(1). Master Theorem case 2 with
       log_b a = 0 and f(n) = n^0 gives O(log n).

   Q4. Iterative vs Recursive -- which is better?
   A.  Iterative. Same time complexity but O(1) space vs O(log n).
       Also no recursion overhead. Recursive is more readable
       for teaching.

   Q5. What if there are duplicates?
   A.  Standard binary search returns ANY match. For first /
       last occurrence, modify: on match, don't return -- save
       index and continue searching the appropriate half.

   ============================================================
   CROSS QUESTIONS
   ---------------
   CQ1. Can binary search work on UNSORTED data?
        No -- the sorted property is essential. But you could
        sort first (O(n log n)) and then search (O(log n)).
        Only worth it if many searches will follow.

   CQ2. How is std::lower_bound different from this?
        lower_bound returns iterator to first element NOT LESS
        than target (so target can be inserted there while
        keeping order). It works even when target is absent.

   CQ3. What's exponential / galloping search?
        For unbounded sorted streams: double the range size
        (1, 2, 4, 8, ...) until you overshoot, then binary
        search within. O(log p) where p is target's position.

   CQ4. How would you binary search on a ROTATED sorted array?
        At each step one half is still sorted -- determine
        which half by comparing arr[low] with arr[mid]. Check
        if target lies in the sorted half; if yes, recurse
        there; else recurse into the other half. Still O(log n).

   CQ5. Can binary search be done in PARALLEL?
        Not effectively -- each step depends on the previous.
        It's inherently sequential. But you can split the array
        across machines for distributed lookup.
   ============================================================ */
