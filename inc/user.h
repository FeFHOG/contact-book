#ifndef USER_H
#define USER_H

#include "contact_book_card.h"
#include "contact_book_phone.h"

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
