#include <bits/stdc++.h>
using namespace std;
struct HuffmanNode {
    char character;
    long long frequency;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char ch, long long freq, HuffmanNode* l = nullptr, HuffmanNode* r = nullptr)
        : character(ch), frequency(freq), left(l), right(r) {}
};

struct CompareNodes {
    bool operator()(HuffmanNode* a, HuffmanNode* b) const {
        return a->frequency > b->frequency;
    }
};

void generateCodes(HuffmanNode* node, string currentCode, string* codes) {
    if (!node) return;

    if (!node->left && !node->right) {
        codes[node->character - 'a'] = currentCode;
    } else {
        generateCodes(node->left, currentCode + "0", codes);
        generateCodes(node->right, currentCode + "1", codes);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    string input;
    getline(cin, input);

    long long freq[26] = {0};
    for (char c : input) {
        if (c >= 'a' && c <= 'z') {
            freq[c - 'a']++;
        }
    }

    priority_queue<HuffmanNode*, vector<HuffmanNode*>, CompareNodes> pq;
    int uniqueChars = 0;

    for (int i = 0; i < 26; ++i) {
        if (freq[i] > 0) {
            pq.push(new HuffmanNode('a' + i, freq[i]));
            uniqueChars++;
        }
    }

    if (pq.empty()) {
        return 0;
    }

    while (pq.size() > 1) {
        HuffmanNode* left = pq.top(); pq.pop();
        HuffmanNode* right = pq.top(); pq.pop();
        pq.push(new HuffmanNode('\0', left->frequency + right->frequency, left, right));
    }

    HuffmanNode* root = pq.top();

    string codes[26];
    if (uniqueChars == 1) {
        for (int i = 0; i < 26; ++i) {
            if (freq[i] > 0) {
                codes[i] = "0";
            }
        }
    } else {
        generateCodes(root, "", codes);
    }

    string encodedString;
    long long totalLength = 0;
    for (char c : input) {
        string code = codes[c - 'a'];
        encodedString += code;
        totalLength += code.length();
    }

    cout << uniqueChars << " " << totalLength << '\n';
    for (int i = 0; i < 26; ++i) {
        if (freq[i] > 0) {
            cout << (char)('a' + i) << ": " << codes[i] << '\n';
        }
    }
    cout << encodedString << '\n';

    return 0;
}