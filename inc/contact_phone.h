#ifndef CONTACT_PHONE_H
#define CONTACT_PHONE_H

#include "contact_card.h"

class ContactPhone : public ContactCard {
private:
    std::string hometown;
    std::string qq;

public:
    // 手机卡联系人：继承基础联系人，并增加籍贯和 QQ。
    ContactPhone(const std::string& name = "", const std::string& phoneNumber = "",
                 const std::string& hometown = "", const std::string& qq = "");

    std::string getHometown() const;
    std::string getQQ() const;

    void setHometown(const std::string& newHometown);
    void setQQ(const std::string& newQQ);

    friend std::ostream& operator<<(std::ostream& os, const ContactPhone& contact);
    friend std::istream& operator>>(std::istream& is, ContactPhone& contact);
};

#endif
