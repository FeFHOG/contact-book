#include "contact_card.h"
#include "contact_validator.h"

#include <limits>

ContactCard::ContactCard(const std::string& name, const std::string& phoneNumber)
    : name(name)
    , phoneNumber(phoneNumber)
{
}

std::string ContactCard::getName() const
{
    return name;
}

std::string ContactCard::getPhoneNumber() const
{
    return phoneNumber;
}

void ContactCard::setName(const std::string& newName)
{
    name = newName;
}

void ContactCard::setPhoneNumber(const std::string& newPhoneNumber)
{
    phoneNumber = newPhoneNumber;
}

std::ostream& operator<<(std::ostream& os, const ContactCard& contact)
{
    os << "姓名: " << contact.name << '\n'
       << "电话号码: " << contact.phoneNumber << '\n';
    return os;
}

std::istream& operator>>(std::istream& is, ContactCard& contact)
{
    std::string name;
    std::string phoneNumber;

    std::cout << "请输入姓名: ";
    std::getline(is >> std::ws, name);
    std::cout << "请输入电话号码: ";
    while (true) {
        std::getline(is, phoneNumber);
        if (isValidPhoneNumber(phoneNumber)) {
            break;
        }
        std::cout << "电话号码格式不合法，请输入 6-20 位数字，可选前缀 +: ";
    }

    contact.setName(name);
    contact.setPhoneNumber(phoneNumber);
    return is;
}
