#ifndef STRING_SORT_TESTER_HPP
#define STRING_SORT_TESTER_HPP

#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include "StringGenerator.hpp"
#include "StringSortAlgorithms.hpp"

class StringSortTester {
public:
    struct TestResult {
        std::string datasetType;
        int size;
        std::map<std::string, SortStats> results;
    };

    using DatasetGenerator = std::function<std::vector<std::string>(int)>;

    void runTests(
        const std::vector<int>& sizes,
        const std::map<std::string, DatasetGenerator>& datasets,
        const std::map<std::string, SortFunction>& algorithms,
        int repeats = 5
    );

    void saveResultsToCSV(const std::string& filename);

private:
    std::vector<TestResult> testResults;
};

void StringSortTester::runTests(
    const std::vector<int>& sizes,
    const std::map<std::string, DatasetGenerator>& datasets,
    const std::map<std::string, SortFunction>& algorithms,
    int repeats
) {
    for (int size : sizes) {
        for (const auto& [type, generator] : datasets) {
            std::cout << "Testing dataset: " << type << ", size: " << size << "\n";
            TestResult result;
            result.datasetType = type;
            result.size = size;

            for (int rep = 0; rep < repeats; ++rep) {
                StringGenerator gen;
                auto data = generator(size);
                for (const auto& [name, sortFunc] : algorithms) {
                    auto copy = data;
                    auto stats = sortFunc(copy);
                    result.results[name].timeMs += stats.timeMs / repeats;
                    result.results[name].comparisons += stats.comparisons / repeats;
                }
            }

            testResults.push_back(result);
        }
    }
}

void StringSortTester::saveResultsToCSV(const std::string& filename) {
    std::ofstream file(filename);
    file << "dataset,size,algorithm,time_ms,comparisons\n";
    for (const auto& res : testResults) {
        for (const auto& [algo, stats] : res.results) {
            file << res.datasetType << ","
                 << res.size << ","
                 << algo << ","
                 << stats.timeMs << ","
                 << stats.comparisons << "\n";
        }
    }
}

#endif // STRING_SORT_TESTER_HPP