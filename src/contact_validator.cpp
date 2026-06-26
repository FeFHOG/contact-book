#include "contact_validator.h"

#include <cctype>

bool isValidPhoneNumber(const std::string& phoneNumber)
{
    if (phoneNumber.empty()) {
        return false;
    }

    int start = 0;
    if (phoneNumber[0] == '+') {
        start = 1;
    }

    int digitCount = static_cast<int>(phoneNumber.size()) - start;
    if (digitCount < 6 || digitCount > 20) {
        return false;
    }

    for (int i = start; i < static_cast<int>(phoneNumber.size()); ++i) {
        if (!std::isdigit(static_cast<unsigned char>(phoneNumber[i]))) {
            return false;
        }
    }

    return true;
}
