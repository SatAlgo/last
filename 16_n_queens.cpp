/* ============================================================
   TOPIC : N-QUEENS
   PARADIGM : Backtracking
   ============================================================

   CONCEPT
   -------
   Place N queens on an N x N chessboard so no two attack each
   other (no two in same row, column, or diagonal). Place
   queens column by column. For each column, try each row in
   turn -- if it's safe, recurse; otherwise backtrack.

   Two queens attack each other iff:
       same row    (row1 == row2)
       same column (col1 == col2)
       same diagonal (|row1-row2| == |col1-col2|)

   We naturally avoid same-column conflicts by placing one
   queen per column.

   BRUTE FORCE
   -----------
   Try every possible placement of N queens in N^2 cells:
   C(N^2, N) -- astronomically large. Even smarter brute force
   (one queen per column = N^N) is much worse than backtracking
   (~N! with pruning).

   COMPLEXITY
   ----------
       Best     : O(N)         lucky placements
       Average  : varies
       Worst    : O(N!)        but pruning makes it MUCH less
       Space    : O(N^2)       board, O(N) recursion + helper arrays

   REAL-WORLD USES
   ---------------
   * Classic teaching example for Constraint Satisfaction
     Problems (CSPs)
   * Parallel task scheduling with non-overlap constraints
   * VLSI floor-planning (placing components with adjacency
     constraints)
   * AI planning with similar constraint structures
   * Symmetry-breaking case studies in operations research
   ============================================================ */

#include <iostream>
#include <vector>
using namespace std;

/* --- BASIC version: O(N) safety check ---------------------- */
bool isSafe(vector<vector<int>>& board, int row, int col, int N) {
    /* row scan to the left */
    for (int i = 0; i < col; i++)
        if (board[row][i]) return false;
    /* upper-left diagonal */
    for (int i = row, j = col; i >= 0 && j >= 0; i--, j--)
        if (board[i][j]) return false;
    /* lower-left diagonal */
    for (int i = row, j = col; j >= 0 && i < N; i++, j--)
        if (board[i][j]) return false;
    return true;
}

bool solveNQueens(vector<vector<int>>& board, int col, int N) {
    if (col >= N) return true;
    for (int i = 0; i < N; i++) {
        if (isSafe(board, i, col, N)) {
            board[i][col] = 1;
            if (solveNQueens(board, col + 1, N)) return true;
            board[i][col] = 0;
        }
    }
    return false;
}

/* --- ADVANCED: tracking sets for O(1) safety check --------- */
int solutionCount = 0;
void solveNQueensFast(int n, int row, vector<bool>& cols,
                      vector<bool>& diag1, vector<bool>& diag2) {
    if (row == n) { solutionCount++; return; }
    for (int col = 0; col < n; col++) {
        int d1 = row - col + n;   // shift to keep non-negative
        int d2 = row + col;
        if (cols[col] || diag1[d1] || diag2[d2]) continue;
        cols[col] = diag1[d1] = diag2[d2] = true;
        solveNQueensFast(n, row + 1, cols, diag1, diag2);
        cols[col] = diag1[d1] = diag2[d2] = false;
    }
}

int main() {
    /* BASIC version on a 4x4 board */
    int N = 4;
    vector<vector<int>> board(N, vector<int>(N, 0));
    cout << "=== BASIC version: one solution for N=4 ===\n";
    if (!solveNQueens(board, 0, N)) cout << "No solution\n";
    else {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++)
                cout << (board[i][j] ? "Q " : ". ");
            cout << "\n";
        }
    }

    /* ADVANCED: count solutions for N = 1..10 */
    cout << "\n=== ADVANCED: total solutions for N = 1..10 ===\n";
    cout << "N | solutions\n";
    cout << "--+----------\n";
    for (int n = 1; n <= 10; n++) {
        solutionCount = 0;
        vector<bool> cols(n, false), diag1(2*n+1, false), diag2(2*n+1, false);
        solveNQueensFast(n, 0, cols, diag1, diag2);
        cout << " " << n << "|  " << solutionCount << "\n";
    }
    /* Expected counts: 1, 0, 0, 2, 10, 4, 40, 92, 352, 724 */
    return 0;
}

/* ============================================================
   LINE-BY-LINE EXPLANATION
   ------------------------
   BASIC isSafe(): checks for an attacking queen in the SAME ROW
   to the LEFT, the UPPER-LEFT DIAGONAL, and the LOWER-LEFT
   DIAGONAL. We only check LEFT because all queens to the right
   haven't been placed yet (we go column by column).

   BASIC solveNQueens(): for each row in the current column,
   try placing a queen. If safe, recurse on next column. On
   failure, remove the queen (backtrack) and try the next row.

   ADVANCED solveNQueensFast(): uses three boolean arrays:
     cols[c]      : column c already used
     diag1[r-c+n] : '\' diagonal (row-col is constant)
     diag2[r+c]   : '/' diagonal (row+col is constant)
   Each safety check becomes O(1) instead of O(N).

   ============================================================
   VIVA QUESTIONS
   --------------
   Q1. Why place column by column (not row by row)?
   A.  Two queens cannot share a column, so building solutions
       column-wise enforces that constraint by CONSTRUCTION --
       no explicit column check needed. Equivalent to placing
       row by row; just a convention.

   Q2. How many solutions exist for N=8?
   A.  92 total. 12 unique up to symmetry (rotation/reflection).

   Q3. What's the smallest N with no solution?
   A.  N=2 and N=3 have no solutions. N=1 trivially works.
       N>=4 always has at least one.

   Q4. Why does the ADVANCED version use 2*n+1 for diagonals?
   A.  Two diagonals: 'r-c' ranges from -(n-1) to n-1 -- 2n-1
       values. We shift by +n to make indices non-negative
       (covers 1 to 2n-1). 'r+c' ranges from 0 to 2n-2 -- 2n-1
       values. Using size 2n+1 is safe with the +n shift.

   Q5. Is there a non-backtracking approach?
   A.  Yes -- for N >= 4, there are CLOSED-FORM constructions
       (Pauls 1874; Polya 1918) that place queens directly.
       But backtracking generalizes to all CSPs, hence the
       teaching focus.

   ============================================================
   CROSS QUESTIONS
   ---------------
   CQ1. How can you SPEED UP by exploiting symmetry?
        First queen's row can be restricted to rows 0..N/2-1
        (top half). Generate solutions for those, then mirror
        them. Roughly halves the search.

   CQ2. How does this relate to GRAPH COLORING?
        N-Queens is a CSP where the variables are queen
        positions and constraints are 'not in same row /col
        /diagonal'. It's a specialized CSP, not directly graph
        coloring.

   CQ3. What's the BITMASK trick for N-Queens?
        Replace cols / diag1 / diag2 booleans with three INT
        bitmasks (one bit per column or diagonal). 'isSafe'
        becomes a bitwise OR; 'place queen' is bitwise OR; each
        is O(1). Faster than boolean arrays in practice for
        N <= 32. (Bitmask version omitted here for clarity.)

   CQ4. Can N-Queens be solved with INTEGER PROGRAMMING?
        Yes -- model 'x[i][j] in {0,1}', row/col/diag sums <= 1,
        sum of all = N. Solvable by ILP solvers but much slower
        than tailored backtracking.

   CQ5. Why does worst case look like O(N!)?
        N choices for first column, ~(N-2) safe choices for the
        second (one column and at least two diagonals
        eliminated), and so on. Approximately N! before pruning;
        much less with good pruning.
   ============================================================ */
