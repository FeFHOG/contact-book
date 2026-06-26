#ifndef CONTACT_BOOK_CARD_H
#define CONTACT_BOOK_CARD_H

#include "contact_book_interface.h"
#include "contact_card.h"

#include <string>
#include <vector>

class ContactBookCard : public ContactBookInterface {
private:
    std::vector<ContactCard> contacts;
    std::string fileName;

    int findIndexByPhone(const std::string& phoneNumber) const;

public:
    explicit ContactBookCard(const std::string& fileName = "data/phone.txt");
    ~ContactBookCard();

    bool addContact(const ContactCard& contact);
    bool updateContact(const std::string& phoneNumber, const ContactCard& contact);
    bool deleteContact(const std::string& phoneNumber) override;
    bool hasContact(const std::string& phoneNumber) const override;
    void findByName(const std::string& name) const;
    void display() const override;
    int size() const override;

    const std::vector<ContactCard>& getContacts() const;

    void loadFromFile();
    void saveToFile() const;
};

#endif
