#include "StringGenerator.hpp"
#include "StringSortAlgorithms.hpp"
#include "StringSortTester.hpp"

int main() {
    StringSortTester tester;

    std::vector<int> sizes;
    for (int i = 100; i <= 3000; i += 100)
        sizes.push_back(i);

    StringGenerator gen;

    std::map<std::string, StringSortTester::DatasetGenerator> datasets = {
        {"random", [&](int n){ return gen.getRandomStrings(n); }},
        {"reversed", [&](int n){ return gen.getReversedSortedStrings(n); }},
        {"nearly_sorted", [&](int n){ return gen.getNearlySortedStrings(n); }}
    };

    std::map<std::string, SortFunction> algorithms = {
        {"standard_quicksort", makeStandardQuickSort()},
        {"standard_mergesort", makeStandardMergeSort()},
        {"ternary_quicksort", makeTernaryQuickSort()},
        {"msd_radix_noswitch", makeMSDRadixSortNoSwitch()},
        {"msd_radix_withswitch", makeMSDRadixSortWithSwitch()}
    };

    tester.runTests(sizes, datasets, algorithms, 5);
    tester.saveResultsToCSV("results.csv");

    std::cout << "All tests completed and saved to results.csv\n";
    return 0;
}