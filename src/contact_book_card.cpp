#include "contact_book_card.h"
#include "search_algorithm.h"

#include <algorithm>
#include <fstream>
#include <iostream>

using std::cout;
using std::string;

ContactBookCard::ContactBookCard(const string& fileName)
    : fileName(fileName)
{
    loadFromFile();
}

ContactBookCard::~ContactBookCard()
{
    saveToFile();
}

int ContactBookCard::findIndexByPhone(const string& phoneNumber) const
{
    // contacts 始终按电话号码升序保存，因此可以用二分查找定位。
    auto it = std::lower_bound(contacts.begin(), contacts.end(), phoneNumber,
        [](const ContactCard& contact, const string& value) {
            return contact.getPhoneNumber() < value;
        });

    if (it != contacts.end() && it->getPhoneNumber() == phoneNumber) {
        return static_cast<int>(it - contacts.begin());
    }
    return -1;
}

bool ContactBookCard::addContact(const ContactCard& contact)
{
    int index = findIndexByPhone(contact.getPhoneNumber());
    if (index != -1) {
        contacts[index] = contact;
        return true;
    }

    if (contacts.size() >= 1000) {
        cout << "手机通讯录容量已满。\n";
        return false;
    }

    // 新联系人按电话号码插入到有序位置，保证后续查找仍可使用二分。
    auto it = std::lower_bound(contacts.begin(), contacts.end(), contact.getPhoneNumber(),
        [](const ContactCard& current, const string& phoneNumber) {
            return current.getPhoneNumber() < phoneNumber;
        });
    contacts.insert(it, contact);
    return true;
}

bool ContactBookCard::updateContact(const string& phoneNumber, const ContactCard& contact)
{
    int index = findIndexByPhone(phoneNumber);
    if (index == -1) {
        return false;
    }
    contacts[index] = contact;
    return true;
}

bool ContactBookCard::deleteContact(const string& phoneNumber)
{
    int index = findIndexByPhone(phoneNumber);
    if (index == -1) {
        return false;
    }
    contacts.erase(contacts.begin() + index);
    return true;
}

bool ContactBookCard::hasContact(const string& phoneNumber) const
{
    return findIndexByPhone(phoneNumber) != -1;
}

void ContactBookCard::findByName(const string& name) const
{
    bool found = false;
    for (const auto& contact : contacts) {
        // 使用 KMP 支持姓名关键字查询，而不是只能完整匹配姓名。
        if (kmpContains(contact.getName(), name)) {
            cout << contact << "------------------------------\n";
            found = true;
        }
    }
    if (!found) {
        cout << "手机通讯录中未找到该姓名。\n";
    }
}

void ContactBookCard::display() const
{
    if (contacts.empty()) {
        cout << "手机通讯录为空。\n";
        return;
    }

    for (const auto& contact : contacts) {
        cout << contact << "------------------------------\n";
    }
}

int ContactBookCard::size() const
{
    return static_cast<int>(contacts.size());
}

const std::vector<ContactCard>& ContactBookCard::getContacts() const
{
    return contacts;
}

void ContactBookCard::loadFromFile()
{
    contacts.clear();

    std::ifstream fin(fileName);
    if (!fin.is_open()) {
        return;
    }

    string name;
    string phoneNumber;
    while (std::getline(fin, name) && std::getline(fin, phoneNumber)) {
        if (!name.empty() && !phoneNumber.empty()) {
            contacts.push_back(ContactCard(name, phoneNumber));
        }
    }

    // 文件中的数据可能不是有序的，读入后统一排序。
    std::sort(contacts.begin(), contacts.end(),
        [](const ContactCard& a, const ContactCard& b) {
            return a.getPhoneNumber() < b.getPhoneNumber();
        });
}

void ContactBookCard::saveToFile() const
{
    std::ofstream fout(fileName);
    if (!fout.is_open()) {
        cout << "无法写入文件: " << fileName << '\n';
        return;
    }

    // 每个手机联系人用两行保存：姓名、电话号码。
    for (const auto& contact : contacts) {
        fout << contact.getName() << '\n'
             << contact.getPhoneNumber() << '\n';
    }
}
