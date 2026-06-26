#include "contact_phone.h"

ContactPhone::ContactPhone(const std::string& name, const std::string& phoneNumber,
                           const std::string& hometown, const std::string& qq)
    : ContactCard(name, phoneNumber)
    , hometown(hometown)
    , qq(qq)
{
}

std::string ContactPhone::getHometown() const
{
    return hometown;
}

std::string ContactPhone::getQQ() const
{
    return qq;
}

void ContactPhone::setHometown(const std::string& newHometown)
{
    hometown = newHometown;
}

void ContactPhone::setQQ(const std::string& newQQ)
{
    qq = newQQ;
}

std::ostream& operator<<(std::ostream& os, const ContactPhone& contact)
{
    os << static_cast<const ContactCard&>(contact)
       << "籍贯: " << contact.hometown << '\n'
       << "QQ: " << contact.qq << '\n';
    return os;
}

std::istream& operator>>(std::istream& is, ContactPhone& contact)
{
    is >> static_cast<ContactCard&>(contact);

    std::string hometown;
    std::string qq;

    std::cout << "请输入籍贯: ";
    std::getline(is >> std::ws, hometown);
    std::cout << "请输入QQ: ";
    std::getline(is, qq);

    contact.setHometown(hometown);
    contact.setQQ(qq);
    return is;
}
