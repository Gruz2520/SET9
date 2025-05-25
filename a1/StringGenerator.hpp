#ifndef STRING_GENERATOR_HPP
#define STRING_GENERATOR_HPP

#include <vector>
#include <string>
#include <random>
#include <algorithm>

class StringGenerator {
private:
    static const std::string CHARSET;
    std::mt19937 rng;

    std::string generateRandomString(int minLen, int maxLen);
    void shuffleVector(std::vector<std::string>& vec);

public:
    StringGenerator(unsigned int seed = std::random_device{}());

    std::vector<std::string> getRandomStrings(int count, int minLen = 10, int maxLen = 200);
    std::vector<std::string> getReversedSortedStrings(int count, int minLen = 10, int maxLen = 200);
    std::vector<std::string> getNearlySortedStrings(int count, double swapRatio = 0.05, int minLen = 10, int maxLen = 200);
};

// Реализация статических переменных
const std::string StringGenerator::CHARSET =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789"
    "!@#%:;^&*()-";

// Конструктор
StringGenerator::StringGenerator(unsigned int seed) : rng(seed) {}

// Генерация случайной строки
std::string StringGenerator::generateRandomString(int minLen, int maxLen) {
    std::uniform_int_distribution<int> lenDist(minLen, maxLen);
    int len = lenDist(rng);
    std::uniform_int_distribution<int> charDist(0, CHARSET.size() - 1);
    std::string str;
    for (int i = 0; i < len; ++i)
        str += CHARSET[charDist(rng)];
    return str;
}

// Перемешивание
void StringGenerator::shuffleVector(std::vector<std::string>& vec) {
    std::shuffle(vec.begin(), vec.end(), rng);
}

// Получить случайные строки
std::vector<std::string> StringGenerator::getRandomStrings(int count, int minLen, int maxLen) {
    std::vector<std::string> result;
    result.reserve(count);
    for (int i = 0; i < count; ++i)
        result.push_back(generateRandomString(minLen, maxLen));
    return result;
}

// Получить обратно отсортированные
std::vector<std::string> StringGenerator::getReversedSortedStrings(int count, int minLen, int maxLen) {
    auto result = getRandomStrings(count, minLen, maxLen);
    std::sort(result.begin(), result.end());
    std::reverse(result.begin(), result.end());
    return result;
}

// Получить почти отсортированные
std::vector<std::string> StringGenerator::getNearlySortedStrings(int count, double swapRatio, int minLen, int maxLen) {
    auto result = getRandomStrings(count, minLen, maxLen);
    std::sort(result.begin(), result.end());

    int swaps = static_cast<int>(count * swapRatio);
    for (int i = 0; i < swaps; ++i) {
        std::uniform_int_distribution<int> indexDist(0, count - 1);
        int a = indexDist(rng), b = indexDist(rng);
        std::swap(result[a], result[b]);
    }

    return result;
}

#endif // STRING_GENERATOR_HPP