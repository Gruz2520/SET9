#ifndef STRING_SORT_ALGORITHMS_HPP
#define STRING_SORT_ALGORITHMS_HPP

#include <vector>
#include <string>
#include <cstdint>
#include <functional>
#include <chrono>

// Структура для хранения результатов тестирования
struct SortStats {
    double timeMs = 0.0; // Время выполнения в миллисекундах
    uint64_t comparisons = 0; // Количество посимвольных сравнений
};

using SortFunction = std::function<SortStats(std::vector<std::string>&)>;

// Глобальные функции подсчёта символьных сравнений
namespace {

static uint64_t charComparisonCounter = 0;

inline void incrementCharComparison() {
    ++charComparisonCounter;
}

void resetCharComparisons() {
    charComparisonCounter = 0;
}

uint64_t getCharComparisons() {
    return charComparisonCounter;
}

inline int charCode(const std::string& s, int d) {
    if (d >= (int)s.size())
        return 0;

    // Обрезаем значение по ALPHABET_SIZE
    return static_cast<int>(
        static_cast<unsigned char>(s[d]) % 74
    ) + 1;
}

int getCommonPrefixLength(const std::string& a, const std::string& b) {
    int len = 0;
    while (len < (int)a.size() && len < (int)b.size() && a[len] == b[len]) {
        ++len;
        incrementCharComparison();
    }
    return len;
}

bool compareByPrefix(const std::string& a, const std::string& b) {
    int prefixLen = getCommonPrefixLength(a, b);
    if (prefixLen < (int)a.size() && prefixLen < (int)b.size()) {
        incrementCharComparison(); // сравнение одного символа
        return a[prefixLen] < b[prefixLen];
    }
    return a.size() < b.size();
}

}

// Standard QuickSort (через std::sort)
SortFunction makeStandardQuickSort() {
    return [](std::vector<std::string>& arr) -> SortStats {
        resetCharComparisons();
        auto start = std::chrono::high_resolution_clock::now();

        std::sort(arr.begin(), arr.end(), [](const std::string& a, const std::string& b) {
            bool result = a < b;
            if (result || b < a) {
                int i = 0;
                while (i < (int)a.size() && i < (int)b.size() && a[i] == b[i])
                    ++i;
                if (i < (int)a.size() || i < (int)b.size())
                    incrementCharComparison();
            }
            return result;
        });

        auto end = std::chrono::high_resolution_clock::now();
        SortStats stats;
        stats.timeMs = std::chrono::duration<double, std::milli>(end - start).count();
        stats.comparisons = getCharComparisons();
        return stats;
    };
}

// Standard MergeSort (через std::stable_sort)
SortFunction makeStandardMergeSort() {
    return [](std::vector<std::string>& arr) -> SortStats {
        resetCharComparisons();
        auto start = std::chrono::high_resolution_clock::now();

        std::stable_sort(arr.begin(), arr.end(), [](const std::string& a, const std::string& b) {
            bool result = compareByPrefix(a, b);
            return result;
        });

        auto end = std::chrono::high_resolution_clock::now();
        SortStats stats;
        stats.timeMs = std::chrono::duration<double, std::milli>(end - start).count();
        stats.comparisons = getCharComparisons();
        return stats;
    };
}

// Тернарная String QuickSort
class StringQuickSort {
private:
    void doSort(std::vector<std::string>& arr, int left, int right, int d) {
        if (left >= right)
            return;

        char pivot = (d < (int)arr[left].size()) ? arr[left][d] : '\0';
        int lt = left, gt = right, i = left;

        while (i <= gt) {
            char current = (d < (int)arr[i].size()) ? arr[i][d] : '\0';
            if (current < pivot)
                std::swap(arr[i++], arr[lt++]);
            else if (current > pivot)
                std::swap(arr[i], arr[gt--]);
            else {
                incrementCharComparison();
                ++i;
            }
        }

        doSort(arr, left, lt - 1, d);
        if (pivot != '\0')
            doSort(arr, lt, gt, d + 1);
        doSort(arr, gt + 1, right, d);
    }

public:
    void sort(std::vector<std::string>& arr) {
        if (!arr.empty())
            doSort(arr, 0, arr.size() - 1, 0);
    }
};

SortFunction makeTernaryQuickSort() {
    StringQuickSort qs;
    return [qs](std::vector<std::string>& arr) mutable -> SortStats {
        resetCharComparisons();
        auto start = std::chrono::high_resolution_clock::now();
        qs.sort(arr);
        auto end = std::chrono::high_resolution_clock::now();
        SortStats stats;
        stats.timeMs = std::chrono::duration<double, std::milli>(end - start).count();
        stats.comparisons = getCharComparisons();
        return stats;
    };
}

// MSD Radix Sort без переключения
class MSDRadixSortNoSwitch {
private:
    static const int ALPHABET_SIZE = 256 + 1;
    std::vector<std::string> aux;

    void sort(std::vector<std::string>& arr, int left, int right, int d) {
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
            arr[left + --count[code + 1]] = std::move(aux[i]);
        }

        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            if (count[i + 1] > count[i])
                sort(arr, left + count[i], left + count[i + 1] - 1, d + 1);
        }
    }

public:
    void operator()(std::vector<std::string>& arr) {
        if (!arr.empty()) {
            aux.reserve(arr.size());
            sort(arr, 0, arr.size() - 1, 0);
        }
    }
};

SortFunction makeMSDRadixSortNoSwitch() {
    MSDRadixSortNoSwitch sorter;
    return [sorter](std::vector<std::string>& arr) mutable -> SortStats {
        resetCharComparisons();
        auto start = std::chrono::high_resolution_clock::now();
        sorter(arr);
        auto end = std::chrono::high_resolution_clock::now();
        SortStats stats;
        stats.timeMs = std::chrono::duration<double, std::milli>(end - start).count();
        stats.comparisons = getCharComparisons();
        return stats;
    };
}

// MSD Radix Sort с переключением на QuickSort
class MSDRadixSortWithSwitch {
private:
    static const int ALPHABET_SIZE = 74;
    StringQuickSort qs;
    void recursiveSort(std::vector<std::string>& arr, int left, int right, int d) {
        if (left >= right || d >= 200)
            return;

        if (right - left + 1 < ALPHABET_SIZE) {
            std::vector<std::string> sub;
            for (int i = left; i <= right; ++i)
                sub.push_back(arr[i]);
            qs.sort(sub);
            for (int i = 0; i < (int)sub.size(); ++i)
                arr[left + i] = std::move(sub[i]);
            return;
        }

        int count[ALPHABET_SIZE + 2] = {0};
        std::vector<std::string> aux(right - left + 1);
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
            arr[next[code]++] = std::move(aux[i]);
        }

        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            if (count[i + 1] > count[i])
                recursiveSort(arr, left + count[i], left + count[i + 1] - 1, d + 1);
        }
    }

public:
    void operator()(std::vector<std::string>& arr) {
        if (!arr.empty())
            recursiveSort(arr, 0, arr.size() - 1, 0);
    }
};

SortFunction makeMSDRadixSortWithSwitch() {
    MSDRadixSortWithSwitch sorter;
    return [sorter](std::vector<std::string>& arr) mutable -> SortStats {
        resetCharComparisons();
        auto start = std::chrono::high_resolution_clock::now();
        sorter(arr);
        auto end = std::chrono::high_resolution_clock::now();
        SortStats stats;
        stats.timeMs = std::chrono::duration<double, std::milli>(end - start).count();
        stats.comparisons = getCharComparisons();
        return stats;
    };
}

#endif // STRING_SORT_ALGORITHMS_HPP