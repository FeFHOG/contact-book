#ifndef USER_H
#define USER_H

#include "contact_book_card.h"
#include "contact_book_phone.h"

// User 组合两个通讯录对象 对main提供菜单功能接口
class User {
private:
    ContactBookCard phoneBook;
    ContactBookPhone cardBook;

public:
    User();

    void addContact();
    void deleteContact();
    void modifyContact();
    void findContactByName() const;
    void displayAll() const;

    void movePhoneToCard();
    void moveCardToPhone();
    void copyPhoneToCard();
    void copyCardToPhone();
};

#endif
