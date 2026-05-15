============================================================
  DAA LAB EXAM -- C++ FILES  (22 topics)
============================================================

Each file is SELF-CONTAINED with:
  * Concept
  * Brute-force method (brief)
  * Working C++ code (basic + advanced where relevant)
  * Inline comments
  * Multiple test cases including edge cases
  * Line-by-line explanation
  * Time + Space complexity (best / avg / worst)
  * Recurrence relation + Master Theorem solution (where applicable)
  * 5 viva questions with answers
  * 5 cross questions with answers
  * Real-world uses

To compile a file:
    g++ -std=c++17 -Wno-vla 01_merge_sort.cpp -o ms && ./ms

------------------------------------------------------------
  CATALOGUE
------------------------------------------------------------

DIVIDE & CONQUER
  01_merge_sort.cpp                  Stable O(n log n) sort, recurrence T(n)=2T(n/2)+O(n)
  02_quick_sort.cpp                  Basic last-pivot quick sort, Lomuto partition
  03_randomized_quick_sort.cpp       Random pivot -- expected O(n log n) for ANY input
  04_quick_sort_median_of_medians.cpp   Deterministic worst-case O(n log n) via MoM pivot
  05_karatsuba.cpp                   O(n^1.585) integer multiplication
  06_strassen.cpp                    O(n^2.807) matrix multiplication
  07_binary_search.cpp               Iterative + recursive + first-occurrence variants

GREEDY
  08_fractional_knapsack.cpp         Sort by value/weight ratio
  09_job_sequencing.cpp              Profit-descending + latest-free-slot
  10_huffman_coding.cpp              Prefix codes; Huffman vs fixed-length comparison output
  11_coin_change_greedy.cpp          Includes failure demo on non-canonical {1,3,4}

DYNAMIC PROGRAMMING
  12_coin_change_dp.cpp              dp[i] = min coins; reconstructs coin list
  13_knapsack_dp.cpp                 2D version + 1D space-optimized

BRANCH AND BOUND
  14_knapsack_branch_bound.cpp       Priority-queue best-first with fractional bound
  15_sum_of_subsets.cpp              Upper + lower bound pruning

BACKTRACKING
  16_n_queens.cpp                    Basic + tracking-set advanced; solution counts N=1..10
  17_graph_coloring.cpp              m-coloring; includes K4 (needs 4 colors) demo

STRING MATCHING
  18_naive_string_matching.cpp       Baseline brute force, O(n*m) worst case
  19_kmp.cpp                         LPS array printed in output (comparison table)
  20_rabin_karp.cpp                  Rolling hash with collision tracking
  21_boyer_moore.cpp                 Bad-character heuristic, right-to-left compare
  22_string_matching_comparison.cpp  ALL 4 ALGORITHMS COMPARED on same input
                                     -- empirical table (comparisons used)
                                     -- conceptual table (preprocessing / time / space)
                                     -- 'when to use what' summary

------------------------------------------------------------
  EXAM TIPS
------------------------------------------------------------

The 'master theorem' application is asked frequently.
Memorize the three cases:
  T(n) = a T(n/b) + f(n),  let c = log_b a
  Case 1: f(n) = O(n^(c - eps))   -> T(n) = Theta(n^c)
  Case 2: f(n) = Theta(n^c)       -> T(n) = Theta(n^c * log n)
  Case 3: f(n) = Omega(n^(c+eps)) -> T(n) = Theta(f(n))

Common recurrences in this syllabus:
  Merge sort:           T(n) = 2T(n/2) + O(n)       -> O(n log n)
  Binary search:        T(n) = T(n/2) + O(1)        -> O(log n)
  Karatsuba:            T(n) = 3T(n/2) + O(n)       -> O(n^1.585)
  Strassen:             T(n) = 7T(n/2) + O(n^2)     -> O(n^2.807)
  Quick sort worst:     T(n) = T(n-1) + O(n)        -> O(n^2)
  Quick sort balanced:  T(n) = 2T(n/2) + O(n)       -> O(n log n)

GOOD LUCK!
============================================================
