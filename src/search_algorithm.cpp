#include "search_algorithm.h"

#include <vector>

namespace {

// 构造 KMP 的 next 数组，记录前后缀能对上多少
std::vector<int> buildNextArray(const std::string& pattern)
{
    std::vector<int> next(pattern.size(), 0);
    for (int i = 1, j = 0; i < static_cast<int>(pattern.size()); ++i) {
        while (j > 0 && pattern[i] != pattern[j]) {
            j = next[j - 1];
        }
        if (pattern[i] == pattern[j]) {
            ++j;
        }
        next[i] = j;
    }
    return next;
}

}

bool kmpContains(const std::string& text, const std::string& pattern)
{
    if (pattern.empty()) {
        return true;
    }

    std::vector<int> next = buildNextArray(pattern);
    for (int i = 0, j = 0; i < static_cast<int>(text.size()); ++i) {
        // 匹配失败时用 next 数组往前跳，不用从头重来
        while (j > 0 && text[i] != pattern[j]) {
            j = next[j - 1];
        }
        if (text[i] == pattern[j]) {
            ++j;
        }
        if (j == static_cast<int>(pattern.size())) {
            return true;
        }
    }
    return false;
}
