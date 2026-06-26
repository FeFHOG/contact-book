#include "user.h"

#include <iostream>

using std::cin;
using std::cout;

void printMenu()
{
    cout << "\n========== 通讯录管理系统 ==========\n";
    cout << "1. 新建联系人\n";
    cout << "2. 删除联系人\n";
    cout << "3. 修改联系人\n";
    cout << "4. 按姓名查询联系人\n";
    cout << "5. 浏览所有联系人\n";
    cout << "6. 将手机联系人移动到手机卡\n";
    cout << "7. 将手机卡联系人移动到手机\n";
    cout << "8. 将手机联系人复制到手机卡\n";
    cout << "9. 将手机卡联系人复制到手机\n";
    cout << "0. 退出系统\n";
    cout << "请选择操作: ";
}

int main()
{
    User user;

    while (true) {
        printMenu();

        int choice;
        if (!(cin >> choice)) {
            cout << "输入错误，程序结束。\n";
            return 0;
        }

        switch (choice) {
        case 1:
            user.addContact();
            break;
        case 2:
            user.deleteContact();
            break;
        case 3:
            user.modifyContact();
            break;
        case 4:
            user.findContactByName();
            break;
        case 5:
            user.displayAll();
            break;
        case 6:
            user.movePhoneToCard();
            break;
        case 7:
            user.moveCardToPhone();
            break;
        case 8:
            user.copyPhoneToCard();
            break;
        case 9:
            user.copyCardToPhone();
            break;
        case 0:
            cout << "感谢使用，再见。\n";
            return 0;
        default:
            cout << "无效选项，请重新输入。\n";
            break;
        }
    }
}
