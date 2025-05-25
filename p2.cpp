#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    getline(cin, s);
    unordered_map<string, int> dictionary;
    for (int i = 0; i < 128; ++i) {
        dictionary[string(1, char(i))] = i;
    }

    vector<int> result;
    string current;
    int dictSize = 128;

    for (char ch : s) {
        string next = current + ch;

        if (dictionary.count(next)) {
            current = next;
        } else {
            result.push_back(dictionary[current]);
            if (dictSize < 65536) {
                dictionary[next] = dictSize++;
            }
            current = ch;
        }
    }

    if (!current.empty()) {
        result.push_back(dictionary[current]);
    }

    cout << result.size() << '\n';
    for (int code : result) {
        cout << code << ' ';
    }
    cout << '\n';

    return 0;
}