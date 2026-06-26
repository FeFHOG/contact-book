#ifndef CONTACT_CARD_H
#define CONTACT_CARD_H

#include <iostream>
#include <string>

class ContactCard {
private:
    std::string name;
    std::string phoneNumber;

public:
    // 基础联系人：用于手机通讯录，只保存姓名和电话号码。
    ContactCard(const std::string& name = "", const std::string& phoneNumber = "");

    std::string getName() const;
    std::string getPhoneNumber() const;

    void setName(const std::string& newName);
    void setPhoneNumber(const std::string& newPhoneNumber);

    friend std::ostream& operator<<(std::ostream& os, const ContactCard& contact);
    friend std::istream& operator>>(std::istream& is, ContactCard& contact);
};

#endif
