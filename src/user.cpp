#include "user.h"

#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::string;

User::User()
    : phoneBook("data/phone.txt")
    , cardBook("data/card.txt")
{
}

void User::addContact()
{
    cout << "请选择存储位置: 1.手机 2.手机卡\n";
    int where;
    cin >> where;

    if (where == 1) {
        ContactCard contact;
        cin >> contact;
        if (phoneBook.addContact(contact)) {
            cout << "手机联系人已保存。\n";
        }
    } else if (where == 2) {
        ContactPhone contact;
        cin >> contact;
        if (cardBook.addContact(contact)) {
            cout << "手机卡联系人已保存。\n";
        }
    } else {
        cout << "无效选择。\n";
    }
}

void User::deleteContact()
{
    cout << "请输入要删除的电话号码: ";
    string phoneNumber;
    std::getline(cin >> std::ws, phoneNumber);

    // 两边如果都有这个号码，就两边都删
    bool deletedPhone = phoneBook.deleteContact(phoneNumber);
    bool deletedCard = cardBook.deleteContact(phoneNumber);

    if (deletedPhone || deletedCard) {
        cout << "删除完成。\n";
    } else {
        cout << "未找到该电话号码。\n";
    }
}

void User::modifyContact()
{
    cout << "请输入要修改的电话号码: ";
    string phoneNumber;
    std::getline(cin >> std::ws, phoneNumber);

    bool found = false;

    // 两边如果都有这个号码，就两边都改
    if (phoneBook.hasContact(phoneNumber)) {
        cout << "修改手机联系人信息:\n";
        ContactCard contact;
        cin >> contact;
        phoneBook.updateContact(phoneNumber, contact);
        found = true;
    }

    if (cardBook.hasContact(phoneNumber)) {
        cout << "修改手机卡联系人信息:\n";
        ContactPhone contact;
        cin >> contact;
        cardBook.updateContact(phoneNumber, contact);
        found = true;
    }

    if (found) {
        cout << "修改完成。\n";
    } else {
        cout << "未找到该电话号码。\n";
    }
}

void User::findContactByName() const
{
    cout << "请输入要查询的姓名: ";
    string name;
    std::getline(cin >> std::ws, name);

    cout << "【手机通讯录查询结果】\n";
    phoneBook.findByName(name);
    cout << "【手机卡通讯录查询结果】\n";
    cardBook.findByName(name);
}

void User::displayAll() const
{
    cout << "【手机通讯录】\n";
    phoneBook.display();
    cout << "【手机卡通讯录】\n";
    cardBook.display();
}

void User::copyPhoneToCard()
{
    if (phoneBook.size() + cardBook.size() > 1000) {
        cout << "手机卡容量不足，无法复制。\n";
        return;
    }

    std::vector<ContactCard> phoneContacts = phoneBook.getContacts();
    for (const ContactCard& contact : phoneContacts) {
        // 已经有的就不重复复制了
        if (!cardBook.hasContact(contact.getPhoneNumber())) {
            cardBook.addContact(ContactPhone(contact.getName(), contact.getPhoneNumber(), "未填写", "未填写"));
        }
    }
    cout << "已将手机联系人复制到手机卡。\n";
}

void User::copyCardToPhone()
{
    if (phoneBook.size() + cardBook.size() > 1000) {
        cout << "手机容量不足，无法复制。\n";
        return;
    }

    std::vector<ContactPhone> cardContacts = cardBook.getContacts();
    for (const ContactPhone& contact : cardContacts) {
        if (!phoneBook.hasContact(contact.getPhoneNumber())) {
            phoneBook.addContact(ContactCard(contact.getName(), contact.getPhoneNumber()));
        }
    }
    cout << "已将手机卡联系人复制到手机。\n";
}

void User::movePhoneToCard()
{
    // 移动就是先复制，再删除原来的
    copyPhoneToCard();

    // 先把号码存下来，不然一边遍历一边删容易乱
    std::vector<string> phoneNumbers;
    std::vector<ContactCard> phoneContacts = phoneBook.getContacts();
    for (const ContactCard& contact : phoneContacts) {
        phoneNumbers.push_back(contact.getPhoneNumber());
    }
    for (const string& phoneNumber : phoneNumbers) {
        phoneBook.deleteContact(phoneNumber);
    }

    cout << "已将手机联系人移动到手机卡。\n";
}

void User::moveCardToPhone()
{
    // 移动就是先复制，再删除原来的
    copyCardToPhone();

    // 先把号码存下来，不然一边遍历一边删容易乱
    std::vector<string> phoneNumbers;
    std::vector<ContactPhone> cardContacts = cardBook.getContacts();
    for (const ContactPhone& contact : cardContacts) {
        phoneNumbers.push_back(contact.getPhoneNumber());
    }
    for (const string& phoneNumber : phoneNumbers) {
        cardBook.deleteContact(phoneNumber);
    }

    cout << "已将手机卡联系人移动到手机。\n";
}
