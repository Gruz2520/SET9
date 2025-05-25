#include <bits/stdc++.h>
using namespace std;
int getCommonPrefixLength(const string& a, const string& b) {
    int len = 0;
    while (len < (int)a.size() && len < (int)b.size() && a[len] == b[len])
        ++len;
    return len;
}

bool compareByPrefix(const string& first, const string& second) {
    int prefixLen = getCommonPrefixLength(first, second);
    if (prefixLen < (int)first.size() && prefixLen < (int)second.size()) {
        return first[prefixLen] < second[prefixLen];
    }
    return first.size() < second.size();
}

void mergePortions(string* data, int leftStart, int midEnd, int rightEnd) {
    int leftSize = midEnd - leftStart + 1;
    int rightSize = rightEnd - midEnd;

    string* leftCopy = new string[leftSize];
    string* rightCopy = new string[rightSize];

    for (int i = 0; i < leftSize; ++i)
        leftCopy[i] = data[leftStart + i];
    for (int j = 0; j < rightSize; ++j)
        rightCopy[j] = data[midEnd + 1 + j];

    int i = 0, j = 0, k = leftStart;

    while (i < leftSize && j < rightSize) {
        if (compareByPrefix(leftCopy[i], rightCopy[j])) {
            data[k++] = leftCopy[i++];
        } else {
            data[k++] = rightCopy[j++];
        }
    }

    while (i < leftSize)
        data[k++] = leftCopy[i++];

    while (j < rightSize)
        data[k++] = rightCopy[j++];

    delete[] leftCopy;
    delete[] rightCopy;
}

void recursiveMergeSort(string* array, int start, int end) {
    if (start >= end)
        return;

    int middle = start + (end - start) / 2;

    recursiveMergeSort(array, start, middle);
    recursiveMergeSort(array, middle + 1, end);

    mergePortions(array, start, middle, end);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    string* input = new string[n];
    for (int i = 0; i < n; ++i)
        cin >> input[i];

    recursiveMergeSort(input, 0, n - 1);

    for (int i = 0; i < n; ++i)
        cout << input[i] << '\n';

    delete[] input;

    return 0;
}