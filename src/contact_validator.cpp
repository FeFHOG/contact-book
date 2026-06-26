#include "contact_validator.h"

#include <cctype>

bool isValidPhoneNumber(const std::string& phoneNumber)
{
    if (phoneNumber.empty()) {
        return false;
    }

    int start = 0;
    if (phoneNumber[0] == '+') {
        // 可以加区号但是只能开头
        start = 1;
    }

    int digitCount = static_cast<int>(phoneNumber.size()) - start;
    if (digitCount < 6 || digitCount > 20) {
        return false;
    }

    for (int i = start; i < static_cast<int>(phoneNumber.size()); ++i) {
        // 使用 unsigned char 转换，避免 char 为负值时传给 isdigit 出现未定义行为  （ai改的）
        if (!std::isdigit(static_cast<unsigned char>(phoneNumber[i]))) {
            return false;
        }
    }

    return true;
}
