#include "contact_book_phone.h"
#include "contact_validator.h"
#include "search_algorithm.h"

#include <fstream>
#include <iostream>

using std::cout;
using std::string;

ContactBookPhone::ContactBookPhone(const string& fileName)
    : fileName(fileName)
{
    // 一开始先把文件里的联系人读进来
    loadFromFile();
}

ContactBookPhone::~ContactBookPhone()
{
    // 程序结束时写回文件
    saveToFile();
}

bool ContactBookPhone::addContact(const ContactPhone& contact)
{
    if (!isValidPhoneNumber(contact.getPhoneNumber())) {
        cout << "电话号码格式不合法，联系人未保存。\n";
        return false;
    }

    // 超过 1000 个就不能再新增
    if (!contacts.contains(contact.getPhoneNumber()) && contacts.size() >= 1000) {
        cout << "手机卡通讯录容量已满。\n";
        return false;
    }

    // 树里面会按电话号码插入，同号码就覆盖
    contacts.insertOrUpdate(contact);
    return true;
}

bool ContactBookPhone::updateContact(const string& phoneNumber, const ContactPhone& contact)
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

bool ContactBookPhone::deleteContact(const string& phoneNumber)
{
    return contacts.remove(phoneNumber);
}

bool ContactBookPhone::hasContact(const string& phoneNumber) const
{
    return contacts.contains(phoneNumber);
}

void ContactBookPhone::findByName(const string& name) const
{
    bool found = false;
    // AVL 树是按电话号码排的，姓名查询只能遍历
    std::vector<ContactPhone> allContacts = contacts.toVector();
    for (const ContactPhone& contact : allContacts) {
        // 这里用 KMP 做包含匹配
        if (kmpContains(contact.getName(), name)) {
            cout << contact << "------------------------------\n";
            found = true;
        }
    }
    if (!found) {
        cout << "手机卡通讯录中未找到该姓名。\n";
    }
}

void ContactBookPhone::display() const
{
    std::vector<ContactPhone> allContacts = contacts.toVector();
    if (allContacts.empty()) {
        cout << "手机卡通讯录为空。\n";
        return;
    }

    for (const ContactPhone& contact : allContacts) {
        cout << contact << "------------------------------\n";
    }
}

int ContactBookPhone::size() const
{
    return contacts.size();
}

std::vector<ContactPhone> ContactBookPhone::getContacts() const
{
    return contacts.toVector();
}

void ContactBookPhone::loadFromFile()
{
    contacts.clear();

    std::ifstream fin(fileName);
    if (!fin.is_open()) {
        return;
    }

    string name;
    string phoneNumber;
    string hometown;
    string qq;
    while (std::getline(fin, name) && std::getline(fin, phoneNumber)
           && std::getline(fin, hometown) && std::getline(fin, qq)) {
        // 读文件时也检查一下，防止文件里本来就有错数据
        if (!name.empty() && isValidPhoneNumber(phoneNumber)) {
            contacts.insertOrUpdate(ContactPhone(name, phoneNumber, hometown, qq));
        }
    }
}

void ContactBookPhone::saveToFile() const
{
    std::ofstream fout(fileName);
    if (!fout.is_open()) {
        cout << "无法写入文件: " << fileName << '\n';
        return;
    }

    // 手机卡联系人用四行保存
    std::vector<ContactPhone> allContacts = contacts.toVector();
    for (const ContactPhone& contact : allContacts) {
        fout << contact.getName() << '\n'
             << contact.getPhoneNumber() << '\n'
             << contact.getHometown() << '\n'
             << contact.getQQ() << '\n';
    }
}
