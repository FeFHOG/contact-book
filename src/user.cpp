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

    // 题目要求：如果两个存储位置都有该联系人，需要同时删除。
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

    // 题目要求：如果两个存储位置都有该联系人，需要同时修改。
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

    for (const auto& contact : phoneBook.getContacts()) {
        // 复制时跳过已有号码，避免重复联系人。
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

    for (const auto& contact : cardBook.getContacts()) {
        // 手机联系人没有籍贯和 QQ，因此只保留姓名和电话号码。
        if (!phoneBook.hasContact(contact.getPhoneNumber())) {
            phoneBook.addContact(ContactCard(contact.getName(), contact.getPhoneNumber()));
        }
    }
    cout << "已将手机卡联系人复制到手机。\n";
}

void User::movePhoneToCard()
{
    copyPhoneToCard();

    // 不能一边遍历 vector 一边删除原元素，先保存号码再统一删除。
    std::vector<string> phoneNumbers;
    for (const auto& contact : phoneBook.getContacts()) {
        phoneNumbers.push_back(contact.getPhoneNumber());
    }
    for (const auto& phoneNumber : phoneNumbers) {
        phoneBook.deleteContact(phoneNumber);
    }

    cout << "已将手机联系人移动到手机卡。\n";
}

void User::moveCardToPhone()
{
    copyCardToPhone();

    // 不能一边遍历 vector 一边删除原元素，先保存号码再统一删除。
    std::vector<string> phoneNumbers;
    for (const auto& contact : cardBook.getContacts()) {
        phoneNumbers.push_back(contact.getPhoneNumber());
    }
    for (const auto& phoneNumber : phoneNumbers) {
        cardBook.deleteContact(phoneNumber);
    }

    cout << "已将手机卡联系人移动到手机。\n";
}
