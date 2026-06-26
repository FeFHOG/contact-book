#ifndef CONTACT_BOOK_INTERFACE_H
#define CONTACT_BOOK_INTERFACE_H

#include <string>

class ContactBookInterface {
public:
    virtual ~ContactBookInterface() = default;

    // 抽象接口用于让不同类型通讯录拥有统一操作，体现多态。
    virtual bool deleteContact(const std::string& phoneNumber) = 0;
    virtual bool hasContact(const std::string& phoneNumber) const = 0;
    virtual void display() const = 0;
    virtual int size() const = 0;
};

#endif
