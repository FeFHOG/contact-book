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
    // 构造时立即读入
    loadFromFile();
}

ContactBookCard::~ContactBookCard()
{
    // 析构时自动写回
    saveToFile();
}

bool ContactBookCard::addContact(const ContactCard& contact)
{
    // 检验是否合法
    if (!isValidPhoneNumber(contact.getPhoneNumber())) {
        cout << "电话号码格式不合法，联系人未保存。\n";
        return false;
    }

    // 新增联系人时检测限制 
    if (!contacts.contains(contact.getPhoneNumber()) && contacts.size() >= 1000) {
        cout << "手机通讯录容量已满。\n";
        return false;
    }

    // AVL 树按电话号码插入或覆盖同号码联系人
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
        // 修改时电话号码变了 需要先删除旧键 再按新键插入
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
    // AVL 树只擅长按电话号码查找 按姓名不行
    for (const auto& contact : contacts.toVector()) {
        // 使用 KMP 可以不完整匹配
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

    for (const auto& contact : allContacts) {
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
        // 读文件时校验 避免历史脏数据进入
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
    for (const auto& contact : contacts.toVector()) {
        fout << contact.getName() << '\n'
             << contact.getPhoneNumber() << '\n';
    }
}
