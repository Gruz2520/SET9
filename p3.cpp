#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int k;
    cin >> k;

    vector<int> codes(k);
    for (int i = 0; i < k; ++i) {
        cin >> codes[i];
    }

    unordered_map<int, string> dictionary;
    for (int i = 0; i < 128; ++i) {
        dictionary[i] = string(1, char(i));
    }

    int dictSize = 128;

    string result;
    string prev = dictionary[codes[0]];
    result += prev;

    for (int i = 1; i < k; ++i) {
        int currCode = codes[i];

        string entry;
        if (dictionary.count(currCode)) {
            entry = dictionary[currCode];
        } else if (currCode == dictSize && !prev.empty()) {
            entry = prev + prev[0];
        } else {
            cerr << "Неверный формат LZW\n";
            return 1;
        }

        result += entry;
        if (dictSize < 65536) {
            dictionary[dictSize++] = prev + entry[0];
        }

        prev = entry;
    }

    cout << result << '\n';

    return 0;
}