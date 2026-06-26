#include "contact_book_card.h"
#include "contact_validator.h"
#include "search_algorithm.h"

#include <fstream>
#include <iostream>

using std::cout;
using std::string;

ContactBookCard::ContactBookCard(const string& fileName)
    : fileName(fileName)
{
    // 一开始先把文件里的联系人读进来
    loadFromFile();
}

ContactBookCard::~ContactBookCard()
{
    // 程序结束时写回文件
    saveToFile();
}

bool ContactBookCard::addContact(const ContactCard& contact)
{
    // 先检查电话号码，不合法就不存
    if (!isValidPhoneNumber(contact.getPhoneNumber())) {
        cout << "电话号码格式不合法，联系人未保存。\n";
        return false;
    }

    // 超过 1000 个就不能再新增
    if (!contacts.contains(contact.getPhoneNumber()) && contacts.size() >= 1000) {
        cout << "手机通讯录容量已满。\n";
        return false;
    }

    // 树里面会按电话号码插入，同号码就覆盖
    contacts.insertOrUpdate(contact);
    return true;
}

bool ContactBookCard::updateContact(const string& phoneNumber, const ContactCard& contact)
{
    if (!contacts.contains(phoneNumber)) {
        return false;
    }

    if (!isValidPhoneNumber(contact.getPhoneNumber())) {
        cout << "电话号码格式不合法，修改失败。\n";
        return false;
    }

    if (phoneNumber != contact.getPhoneNumber()) {
        // 如果电话号码也改了，就先删旧号码
        contacts.remove(phoneNumber);
    }
    contacts.insertOrUpdate(contact);
    return true;
}

bool ContactBookCard::deleteContact(const string& phoneNumber)
{
    return contacts.remove(phoneNumber);
}

bool ContactBookCard::hasContact(const string& phoneNumber) const
{
    return contacts.contains(phoneNumber);
}

void ContactBookCard::findByName(const string& name) const
{
    bool found = false;
    // AVL 树是按电话号码排的，姓名查询只能遍历
    std::vector<ContactCard> allContacts = contacts.toVector();
    for (const ContactCard& contact : allContacts) {
        // 这里用 KMP 做包含匹配
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
    std::vector<ContactCard> allContacts = contacts.toVector();
    if (allContacts.empty()) {
        cout << "手机通讯录为空。\n";
        return;
    }

    for (const ContactCard& contact : allContacts) {
        cout << contact << "------------------------------\n";
    }
}

int ContactBookCard::size() const
{
    return contacts.size();
}

std::vector<ContactCard> ContactBookCard::getContacts() const
{
    return contacts.toVector();
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
        // 读文件时也检查一下，防止文件里本来就有错数据
        if (!name.empty() && isValidPhoneNumber(phoneNumber)) {
            contacts.insertOrUpdate(ContactCard(name, phoneNumber));
        }
    }
}

void ContactBookCard::saveToFile() const
{
    std::ofstream fout(fileName);
    if (!fout.is_open()) {
        cout << "无法写入文件: " << fileName << '\n';
        return;
    }

    // 每个手机联系人用两行保存
    std::vector<ContactCard> allContacts = contacts.toVector();
    for (const ContactCard& contact : allContacts) {
        fout << contact.getName() << '\n'
             << contact.getPhoneNumber() << '\n';
    }
}
