/* ============================================================
   TOPIC : HUFFMAN CODING
   PARADIGM : Greedy
   ============================================================

   CONCEPT
   -------
   Build a variable-length PREFIX code where frequent characters
   get SHORT codes and rare ones get long codes. Steps:
     1. Count frequency of each character.
     2. Build a min-heap of nodes (frequency, char).
     3. Extract the TWO smallest nodes, merge into a parent
        whose frequency is their sum. Push back.
     4. Repeat until one node remains -- the Huffman TREE.
     5. Codes are paths from root: left = 0, right = 1.

   The output also shows COMPARISON with fixed-length (3-bit)
   encoding so you can see the compression.

   BRUTE FORCE
   -----------
   Try every possible binary tree shape with these leaves and
   compute total bits = sum(freq * depth). Exponential in number
   of distinct chars. Greedy (always merge two smallest) skips
   this entire search.

   COMPLEXITY
   ----------
       Best     : O(n log n)   n = unique chars; heap ops dominate
       Average  : O(n log n)
       Worst    : O(n log n)
       Space    : O(n)         tree storage and code map

   PROOF (sketch)
   --------------
   Exchange argument: the two LEAST FREQUENT characters appear
   at the GREATEST depth in some optimal tree, and they can be
   made siblings without loss. Therefore merging them into a
   composite frequency is an optimal first step. Induction
   gives optimality of the whole construction.

   REAL-WORLD USES
   ---------------
   * ZIP, GZIP, PNG (DEFLATE = Huffman + LZ77)
   * JPEG image compression, MP3 audio encoding
   * HTTP/2 header compression (HPACK uses Huffman)
   * Morse code is conceptually similar (frequent letters
     have shorter codes, but Morse isn't strictly prefix-free)
   ============================================================ */

#include <iostream>
#include <queue>
#include <unordered_map>
#include <string>
using namespace std;

struct Node {
    char ch;
    int freq;
    Node *left, *right;
    Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
};

struct Compare {
    bool operator()(Node* a, Node* b) { return a->freq > b->freq; }
};

void generateCodes(Node* root, string code,
                   unordered_map<char, string>& huffCode) {
    if (!root) return;
    if (!root->left && !root->right) { huffCode[root->ch] = code; return; }
    generateCodes(root->left,  code + "0", huffCode);
    generateCodes(root->right, code + "1", huffCode);
}

void huffmanCoding(string text) {
    /* 1. frequency map */
    unordered_map<char, int> freq;
    for (char c : text) freq[c]++;

    /* 2. min-heap of nodes ordered by freq */
    priority_queue<Node*, vector<Node*>, Compare> pq;
    for (auto& p : freq) pq.push(new Node(p.first, p.second));

    /* 3. merge two smallest until one remains */
    while (pq.size() > 1) {
        Node* left  = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();
        Node* merged = new Node('$', left->freq + right->freq);
        merged->left  = left;
        merged->right = right;
        pq.push(merged);
    }

    Node* root = pq.top();
    unordered_map<char, string> huffCode;
    generateCodes(root, "", huffCode);

    /* === OUTPUT ============================================ */
    cout << "Input string: \"" << text << "\"  (length="
         << text.length() << ")\n\n";

    cout << "Char | Freq | Huffman Code | Fixed 3-bit\n";
    cout << "-----+------+--------------+------------\n";
    int idx = 0;
    unordered_map<char, string> fixedCode;
    for (auto& p : huffCode) {
        // generate a fake fixed-length code using char index in iteration
        string fixed = "";
        int x = idx;
        for (int b = 2; b >= 0; b--) fixed += ((x >> b) & 1) ? '1' : '0';
        fixedCode[p.first] = fixed;
        cout << "  " << p.first << "  |  " << freq[p.first]
             << "   | " << p.second
             << string(13 - p.second.size(), ' ') << "| " << fixed << "\n";
        idx++;
    }

    cout << "\nEncoded (Huffman): ";
    int huffBits = 0;
    for (char c : text) { cout << huffCode[c]; huffBits += huffCode[c].size(); }
    cout << "\n";

    cout << "Encoded (Fixed)  : ";
    int fixedBits = 0;
    for (char c : text) { cout << fixedCode[c]; fixedBits += 3; }
    cout << "\n\n";

    cout << "Bits used (Huffman): " << huffBits << "\n";
    cout << "Bits used (Fixed)  : " << fixedBits << "\n";
    cout << "Savings            : "
         << (100.0 * (fixedBits - huffBits) / fixedBits) << " %\n";
}

int main() {
    huffmanCoding("huffman");
    cout << "\n=== Second test ===\n\n";
    huffmanCoding("aabbbcccccddddddd");
    return 0;
}

/* ============================================================
   LINE-BY-LINE EXPLANATION
   ------------------------
   freq map: scans the text once, counts each character.

   priority_queue with custom Compare: a MIN-HEAP keyed on
   frequency. We pop the two smallest nodes each iteration.

   Merging: create a composite node with the SUM of the two
   children's frequencies. Push back into the heap.

   generateCodes: DFS. Going left appends '0' to the code,
   right appends '1'. When we hit a leaf, that's the code for
   the leaf's character.

   The OUTPUT block builds a fixed-length 3-bit code for each
   character (just enough since at most 8 distinct chars in
   demo) and shows side-by-side compression.

   ============================================================
   VIVA QUESTIONS
   --------------
   Q1. What is the prefix property and why does it matter?
   A.  No code is a prefix of another. Without it, decoding
       would be ambiguous: if 'A' = 0 and 'B' = 01, the bit
       string '01' could mean B or AB. The prefix property
       ensures unambiguous decoding without delimiters.

   Q2. Is Huffman optimal? Optimal in WHAT sense?
   A.  Optimal among PREFIX codes for a given frequency
       distribution. No other prefix code can encode fewer
       bits. Non-prefix techniques like arithmetic coding can
       beat it in real systems by exploiting fractional bits.

   Q3. When does Huffman NOT help?
   A.  When all characters have nearly EQUAL frequencies --
       Huffman gives fixed-length codes (no compression). Also
       on already-compressed data (high entropy) and on truly
       random data.

   Q4. Static vs adaptive Huffman?
   A.  Static: compute frequencies first (two-pass), build the
       tree once. Adaptive: update the tree as data streams in
       (one-pass) -- useful for live audio/video where you
       can't see the whole input first.

   Q5. Complexity?
   A.  O(n log n) where n is the number of DISTINCT characters.
       Building the heap is O(n); each of the n-1 merges is
       O(log n) for two extract-min and one insert.

   ============================================================
   CROSS QUESTIONS
   ---------------
   CQ1. What if two chars have the same frequency?
        Tie-breaking is arbitrary -- doesn't affect optimality
        (only the SHAPE of the tree).

   CQ2. Can you have HUFFMAN codes longer than the original
        char's bit width?
        Yes! If a character is very rare, its Huffman code can
        be longer than 8 bits. The savings come from common
        characters being MUCH shorter.

   CQ3. How is the Huffman tree TRANSMITTED with the file?
        Two ways: (a) store the frequency table in the header
        (decoder rebuilds tree), (b) store the tree shape
        directly using a canonical encoding. ZIP uses (b).

   CQ4. Entropy and Huffman?
        Shannon's entropy H = -sum(p_i * log p_i) is the
        theoretical lower bound on average bits per symbol.
        Huffman achieves at most H + 1 bits per symbol --
        never beats theoretical optimum but comes very close.

   CQ5. Arithmetic coding vs Huffman?
        Arithmetic coding represents the ENTIRE message as a
        single fraction in [0,1), allowing fractional bits per
        symbol -- it can achieve closer to the entropy bound,
        especially for skewed distributions. More complex and
        has patents history; Huffman is simpler and still
        widely used.
   ============================================================ */
