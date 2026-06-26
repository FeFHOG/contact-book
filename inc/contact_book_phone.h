#ifndef CONTACT_BOOK_PHONE_H
#define CONTACT_BOOK_PHONE_H

#include "contact_book_interface.h"
#include "contact_phone.h"

#include <string>
#include <vector>

class ContactBookPhone : public ContactBookInterface {
private:
    std::vector<ContactPhone> contacts;
    std::string fileName;

    int findIndexByPhone(const std::string& phoneNumber) const;

public:
    explicit ContactBookPhone(const std::string& fileName = "data/card.txt");
    ~ContactBookPhone();

    bool addContact(const ContactPhone& contact);
    bool updateContact(const std::string& phoneNumber, const ContactPhone& contact);
    bool deleteContact(const std::string& phoneNumber) override;
    bool hasContact(const std::string& phoneNumber) const override;
    void findByName(const std::string& name) const;
    void display() const override;
    int size() const override;

    const std::vector<ContactPhone>& getContacts() const;

    void loadFromFile();
    void saveToFile() const;
};

#endif
