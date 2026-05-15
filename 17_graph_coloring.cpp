/* ============================================================
   TOPIC : GRAPH COLORING (m-Coloring)
   PARADIGM : Backtracking
   ============================================================

   CONCEPT
   -------
   Given an undirected graph and m colors, color every vertex
   so that no two ADJACENT vertices share a color. Strategy:
   for each vertex in turn, try each color 1..m. If safe (no
   neighbor has that color), recurse on next vertex. Backtrack
   on failure.

   BRUTE FORCE
   -----------
   Try all m^V colorings, check validity for each. O(m^V * E)
   time. Backtracking prunes invalid partial colorings early.

   COMPLEXITY
   ----------
       Best     : O(m * V)     lucky early success
       Average  : varies
       Worst    : O(m^V)        no valid coloring
       Space    : O(V)         color array + recursion

   CHROMATIC NUMBER
   ----------------
   Smallest m for which a valid coloring exists. Computing it
   is NP-hard. For PLANAR graphs, <= 4 (Four Color Theorem).
   For BIPARTITE graphs, exactly 2.

   REAL-WORLD USES
   ---------------
   * COMPILER REGISTER ALLOCATION: variables = vertices, 'live
     at the same time' = edge, colors = CPU registers.
   * EXAM SCHEDULING: two exams sharing students must be in
     different slots.
   * FREQUENCY ASSIGNMENT for Wi-Fi channels / radio towers
     (avoid interference).
   * MAP COLORING -- the original motivation.
   * SUDOKU is constrained graph coloring (9 colors, 81 cells).
   ============================================================ */

#include <iostream>
#include <vector>
using namespace std;

bool isSafe(int v, vector<vector<int>>& graph, vector<int>& color,
            int c, int V) {
    for (int i = 0; i < V; i++)
        if (graph[v][i] && color[i] == c) return false;
    return true;
}

bool graphColoringUtil(vector<vector<int>>& graph, int m,
                       vector<int>& color, int v, int V) {
    if (v == V) return true;
    for (int c = 1; c <= m; c++) {
        if (isSafe(v, graph, color, c, V)) {
            color[v] = c;
            if (graphColoringUtil(graph, m, color, v + 1, V)) return true;
            color[v] = 0;             // backtrack
        }
    }
    return false;
}

bool graphColoring(vector<vector<int>>& graph, int m, int V) {
    vector<int> color(V, 0);
    if (!graphColoringUtil(graph, m, color, 0, V)) {
        cout << "No solution with " << m << " colors.\n";
        return false;
    }
    cout << "Coloring (" << m << " colors):\n";
    for (int i = 0; i < V; i++)
        cout << "  Vertex " << i << " -> Color " << color[i] << "\n";
    return true;
}

int main() {
    /* graph:
         0 -- 1
         |  / |
         | /  |
         2 -- 3
       Adjacency matrix below. */
    vector<vector<int>> graph = {
        {0, 1, 1, 1},
        {1, 0, 1, 0},
        {1, 1, 0, 1},
        {1, 0, 1, 0}
    };
    int V = 4;

    cout << "Try m = 3:\n";
    graphColoring(graph, 3, V);          /* feasible */

    cout << "\nTry m = 2:\n";
    graphColoring(graph, 2, V);          /* should fail */

    /* K4 (complete graph on 4 vertices) -- needs 4 colors */
    vector<vector<int>> K4 = {
        {0,1,1,1},
        {1,0,1,1},
        {1,1,0,1},
        {1,1,1,0}
    };
    cout << "\nComplete graph K4 with m = 3:\n";
    graphColoring(K4, 3, 4);             /* impossible */
    cout << "\nComplete graph K4 with m = 4:\n";
    graphColoring(K4, 4, 4);             /* possible */
    return 0;
}

/* ============================================================
   LINE-BY-LINE EXPLANATION
   ------------------------
   isSafe(): scans all neighbors of v (via adjacency matrix
   row). If any neighbor already has color c, conflict -> false.

   graphColoringUtil(): for vertex v, try every color 1..m.
   If safe, assign color and recurse on v+1. If recursion
   succeeds, propagate true. Else clear color and try next.

   graphColoring(): entry point. Initialize color array to 0
   (uncolored), call the util on vertex 0.

   ============================================================
   VIVA QUESTIONS
   --------------
   Q1. What is CHROMATIC NUMBER?
   A.  The MINIMUM m for which a valid m-coloring exists.
       Computing it is NP-hard. For PLANAR graphs it's at most
       4 (Four Color Theorem). For BIPARTITE graphs, exactly 2.

   Q2. How can you speed up backtracking here?
   A.  (1) Order vertices by DEGREE (most-constrained first --
       fail fast).  (2) FORWARD CHECKING -- after coloring v,
       verify remaining vertices still have valid color
       options.  (3) SYMMETRY BREAKING -- the first vertex can
       always be color 1.

   Q3. When does graph coloring fail?
   A.  When the chromatic number > m. Complete graph K4 needs
       4 colors -- any m < 4 fails.

   Q4. Is 2-coloring easier than general m-coloring?
   A.  Much easier! 2-coloring = testing if graph is BIPARTITE,
       which is O(V+E) using BFS/DFS. The jump from 2 to 3
       colors is where the problem becomes NP-Complete.

   Q5. Time complexity?
   A.  Worst case O(m^V) -- m choices per vertex, V vertices.
       Backtracking prunes most of this in practice, but worst
       case remains exponential.

   ============================================================
   CROSS QUESTIONS
   ---------------
   CQ1. Why is graph coloring NP-Complete?
        Karp's 21 NP-Complete problems include 3-coloring,
        which reduces from 3-SAT. By extension, m-coloring for
        m >= 3 is NP-Complete.

   CQ2. WELSH-POWELL heuristic for coloring?
        Sort vertices by degree DESCENDING. Color greedily:
        give the first uncolored vertex color 1, then color
        every non-adjacent uncolored vertex with the same
        color. Repeat with color 2, etc. Polynomial-time
        APPROXIMATION (no optimality guarantee).

   CQ3. How is REGISTER ALLOCATION done in compilers?
        Build an INTERFERENCE GRAPH: variables = vertices, edge
        between two variables that are LIVE at the same time.
        Color the graph with k colors = k CPU registers. If
        chromatic number > k, SPILL some variables to memory.

   CQ4. Is checking IF a graph is 3-colorable easier than
        COMPUTING a 3-coloring?
        No -- both are NP-Complete. Decision and search
        versions are polynomial-time equivalent.

   CQ5. Why does the Four Color Theorem hold for PLANAR but
        not general graphs?
        Planarity restricts which adjacencies are possible.
        K5 isn't planar -- you can't embed it without crossing
        edges. The Four Color Theorem requires that
        restriction. For general graphs, K_n needs n colors.
   ============================================================ */
