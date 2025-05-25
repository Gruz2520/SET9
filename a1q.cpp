#include <bits/stdc++.h>
using namespace std;

inline int charCode(const string& s, int d) {
    return (static_cast<size_t>(d) < s.size())
           ? static_cast<unsigned char>(s[d]) + 1
           : 0;
}

class StringQuickSort {
private:
    char getChar(const string& s, int pos) const {
        return pos < (int)s.size() ? s[pos] : '\0';
    }

    void doSort(vector<string>& arr, int left, int right, int d) {
        if (left >= right)
            return;

        char pivot = getChar(arr[left], d);

        int lt = left, gt = right, i = left;

        while (i <= gt) {
            char current = getChar(arr[i], d);
            if (current < pivot)
                swap(arr[i++], arr[lt++]);
            else if (current > pivot)
                swap(arr[i], arr[gt--]);
            else
                ++i;
        }

        doSort(arr, left, lt - 1, d);
        if (pivot != '\0')
            doSort(arr, lt, gt, d + 1);
        doSort(arr, gt + 1, right, d);
    }

public:
    void sort(vector<string>& arr) {
        if (!arr.empty())
            doSort(arr, 0, arr.size() - 1, 0);
    }
};

class MSDRadixWithQuickSwitch {
private:
    static const int ALPHABET_SIZE = 74; 
    StringQuickSort quickSort;

    void recursiveSort(vector<string>& arr, int left, int right, int d) {
        if (left >= right || d >= 200)
            return;

        if (right - left + 1 < ALPHABET_SIZE) {
            vector<string> sub;
            for (int i = left; i <= right; ++i)
                sub.push_back(arr[i]);

            quickSort.sort(sub);
            for (int i = 0; i < (int)sub.size(); ++i)
                arr[left + i] = move(sub[i]);

            return;
        }

        int count[ALPHABET_SIZE + 2] = {0};
        vector<string> aux(right - left + 1);

        for (int i = 0; i < (int)aux.size(); ++i)
            aux[i] = arr[left + i];

        for (int i = 0; i < (int)aux.size(); ++i) {
            int code = charCode(aux[i], d);
            count[code + 1]++;
        }

        int next[ALPHABET_SIZE + 2] = {};
        next[0] = left;
        for (int i = 0; i < ALPHABET_SIZE + 1; ++i)
            next[i + 1] = next[i] + count[i];

        for (int i = 0; i < (int)aux.size(); ++i) {
            int code = charCode(aux[i], d);
            arr[next[code]++] = move(aux[i]);
        }

        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            if (count[i + 1] > count[i]) {
                recursiveSort(arr, left + count[i], left + count[i + 1] - 1, d + 1);
            }
        }
    }

public:
    void operator()(vector<string>& arr) {
        if (!arr.empty())
            recursiveSort(arr, 0, arr.size() - 1, 0);
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

    MSDRadixWithQuickSwitch()(arr);

    for (auto& s : arr)
        cout << s << '\n';

    return 0;
}