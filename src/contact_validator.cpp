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
        // 这里转一下类型，不然有些字符传给 isdigit 可能出问题
        if (std::isdigit(static_cast<unsigned char>(phoneNumber[i])) == 0) {
            return false;
        }
    }

    return true;
}
