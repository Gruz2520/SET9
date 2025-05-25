#include <bits/stdc++.h>
using namespace std;

inline int charCode(const string& s, int d) {
    return (static_cast<size_t>(d) < s.size())
           ? static_cast<unsigned char>(s[d]) + 1
           : 0;
}

class MSDRadixSort {
private:
    static const int ALPHABET_SIZE = 256 + 1; 
    vector<string> aux;

    void sort(vector<string>& arr, int left, int right, int d) {
        if (left >= right || d >= 200)
            return;

        int count[ALPHABET_SIZE + 2] = {0};
        aux.assign(arr.begin() + left, arr.begin() + right + 1);

        for (int i = 0; i < (int)aux.size(); ++i) {
            int code = charCode(aux[i], d);
            count[code + 1]++;
        }

        for (int r = 0, i = 0; i <= ALPHABET_SIZE; ++i)
            r += count[i], count[i] = r;

        for (int i = 0; i < (int)aux.size(); ++i) {
            int code = charCode(aux[i], d);
            arr[left + --count[code + 1]] = move(aux[i]);
        }

        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            if (count[i + 1] > count[i]) {
                sort(arr, left + count[i], left + count[i + 1] - 1, d + 1);
            }
        }
    }

public:
    void operator()(vector<string>& arr) {
        if (!arr.empty()) {
            aux.reserve(arr.size());
            sort(arr, 0, arr.size() - 1, 0);
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<string> arr(n);
    for (int i = 0; i < n; ++i)
        cin >> arr[i];

    MSDRadixSort()(arr);

    for (const auto& s : arr)
        cout << s << '\n';

    return 0;
}