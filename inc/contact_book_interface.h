#ifndef CONTACT_BOOK_INTERFACE_H
#define CONTACT_BOOK_INTERFACE_H

#include <string>

class ContactBookInterface {
public:
    virtual ~ContactBookInterface() = default;

    // 这个接口让手机和手机卡都能用差不多的操作
    virtual bool deleteContact(const std::string& phoneNumber) = 0;
    virtual bool hasContact(const std::string& phoneNumber) const = 0;
    virtual void display() const = 0;
    virtual int size() const = 0;
};

#endif
