#ifndef CONTACT_TREE_H
#define CONTACT_TREE_H

#include <algorithm>
#include <string>
#include <vector>

// 通用 AVL 树 
// 用模板统一管理两类联系人 
template <typename T>
class ContactTree {
private:
    /* 
    AVL 树节点
    data  联系人对象
    height 计算平衡因子
    同一棵树模板可复用给手机联系人和手机卡联系人
    */ 
    struct Node {
        T data;
        Node* left = nullptr;
        Node* right = nullptr;
        int height = 1;

        explicit Node(const T& data)
            : data(data)
        {
        }
    };

    Node* root = nullptr;
    int nodeCount = 0;

    // 空节点高度 0 叶节点 1
    int height(Node* node) const
    {
        if (node) {
            return node->height;
        }
        return 0;
    }

    // AVL 平衡因子 = 左子树高度 - 右子树高度
    //  > 1 or < -1 说明需要旋转调整
    int balanceFactor(Node* node) const
    {
        if (node) {
            return height(node->left) - height(node->right);
        }
        return 0;
    }

    // 自底向上更新节点高度
    void updateHeight(Node* node)
    {
        if (node) {
            node->height = std::max(height(node->left), height(node->right)) + 1;
        }
    }

    Node* rotateRight(Node* y)//修复失衡
    {
        // 右旋
        Node* x = y->left;
        Node* movedSubtree = x->right;

        x->right = y;
        y->left = movedSubtree;

        updateHeight(y);
        updateHeight(x);
        return x;
    }

    Node* rotateLeft(Node* x)
    {
        // 左旋
        Node* y = x->right;
        Node* movedSubtree = y->left;

        y->left = x;
        x->right = movedSubtree;

        updateHeight(x);
        updateHeight(y);
        return y;
    }

    Node* rebalance(Node* node)
    {
        // 先更新高度 then根据平衡因子决定是否旋转
        updateHeight(node);
        int balance = balanceFactor(node);

        if (balance > 1) {
            // 左子树更高
            if (balanceFactor(node->left) < 0) {
                node->left = rotateLeft(node->left);
            }
            return rotateRight(node);
        }

        if (balance < -1) {
            // 右子树更高
            if (balanceFactor(node->right) > 0) {
                node->right = rotateRight(node->right);
            }
            return rotateLeft(node);
        }

        return node;
    }

    Node* insert(Node* node, const T& contact, bool& added)
    {
        // 按电话号码作为关键字插入 保证左子树号码更小
        if (!node) {
            added = true;
            ++nodeCount;
            return new Node(contact);
        }

        const std::string phoneNumber = contact.getPhoneNumber();
        const std::string currentNumber = node->data.getPhoneNumber();
        if (phoneNumber < currentNumber) {
            node->left = insert(node->left, contact, added);
        } else if (phoneNumber > currentNumber) {
            node->right = insert(node->right, contact, added);
        } else {
            // 电话号码相同视为同一联系人 覆盖
            node->data = contact;
            return node;
        }

        //回溯恢复 AVL 平衡
        return rebalance(node);
    }

    Node* minNode(Node* node) const
    {
        // 当前子树中最小电话号码节点一定在最左侧
        while (node && node->left) {
            node = node->left;
        }
        return node;
    }

    Node* remove(Node* node, const std::string& phoneNumber, bool& removed)
    {
        // 删除同样按电话号码搜索目标节点。
        if (!node) {
            return nullptr;
        }

        if (phoneNumber < node->data.getPhoneNumber()) {
            node->left = remove(node->left, phoneNumber, removed);
        } else if (phoneNumber > node->data.getPhoneNumber()) {
            node->right = remove(node->right, phoneNumber, removed);
        } else {
            removed = true;
            if (!node->left || !node->right) {
                // 只有 0 or 1 子时，直接用子替换当前节点。
                Node* child = node->left;
                if (!child) {
                    child = node->right;
                }
                delete node;
                --nodeCount;
                return child;
            }

            // 有2子时 用右子树最小节点作为后继节点替换当前节点 
            //保留二叉搜索树的有序性 便于删除后继节点
            Node* successor = minNode(node->right);
            node->data = successor->data;
            bool removedSuccessor = false;
            node->right = remove(node->right, successor->data.getPhoneNumber(), removedSuccessor);
            removed = true;
        }

        //回溯时重新调整。
        return rebalance(node);
    }

    T* find(Node* node, const std::string& phoneNumber) const
    {
        // 查找
        if (!node) {
            return nullptr;
        }

        if (phoneNumber < node->data.getPhoneNumber()) {
            return find(node->left, phoneNumber);
        }
        if (phoneNumber > node->data.getPhoneNumber()) {
            return find(node->right, phoneNumber);
        }
        return &node->data;
    }

    void inorder(Node* node, std::vector<T>& contacts) const
    {
        // 中序遍历
        if (!node) {
            return;
        }
        inorder(node->left, contacts);
        contacts.push_back(node->data);
        inorder(node->right, contacts);
    }

    void clear(Node* node)
    {
        // 释放节点
        if (!node) {
            return;
        }
        clear(node->left);
        clear(node->right);
        delete node;
    }

public:
    ContactTree() = default;

    ~ContactTree()
    {
        clear();
    }

    ContactTree(const ContactTree&) = delete;
    ContactTree& operator=(const ContactTree&) = delete;

    bool insertOrUpdate(const T& contact)
    {
        // 返回值表示是否插入了新节点 if only 覆盖同号码联系人返回0
        bool added = false;
        root = insert(root, contact, added);
        return added;
    }

    bool remove(const std::string& phoneNumber)
    {
        bool removed = false;
        root = remove(root, phoneNumber, removed);
        return removed;
    }

    T* find(const std::string& phoneNumber) const
    {
        return find(root, phoneNumber);
    }

    bool contains(const std::string& phoneNumber) const
    {
        return find(phoneNumber) != nullptr;
    }

    std::vector<T> toVector() const
    {
        // 对外仍提供 vector 形式 便于操作
        std::vector<T> contacts;
        contacts.reserve(nodeCount);
        inorder(root, contacts);
        return contacts;
    }

    int size() const
    {
        return nodeCount;
    }

    void clear()
    {
        clear(root);
        root = nullptr;
        nodeCount = 0;
    }
};

#endif
