#ifndef CONTACT_TREE_H
#define CONTACT_TREE_H

#include <algorithm>
#include <string>
#include <vector>

// 通用 AVL 树
// 这里用模板是为了手机和手机卡两种联系人都能放
template <typename T>
class ContactTree {
private:
    /* 
    AVL 树节点
    data 放联系人
    height 记录高度，后面算平衡因子要用
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
        if (node != nullptr) {
            return node->height;
        }
        return 0;
    }

    // 平衡因子就是左边高度减右边高度
    int balanceFactor(Node* node) const
    {
        if (node != nullptr) {
            return height(node->left) - height(node->right);
        }
        return 0;
    }

    // 改完左右儿子后要重新算高度
    void updateHeight(Node* node)
    {
        if (node != nullptr) {
            node->height = std::max(height(node->left), height(node->right)) + 1;
        }
    }

    Node* rotateRight(Node* y)
    {
        // 右旋，用来修左边太高的情况
        Node* x = y->left;
        Node* temp = x->right;

        x->right = y;
        y->left = temp;

        updateHeight(y);
        updateHeight(x);
        return x;
    }

    Node* rotateLeft(Node* x)
    {
        // 左旋，用来修右边太高的情况
        Node* y = x->right;
        Node* temp = y->left;

        y->left = x;
        x->right = temp;

        updateHeight(x);
        updateHeight(y);
        return y;
    }

    Node* rebalance(Node* node)
    {
        // 先更新高度，再看看要不要旋转
        updateHeight(node);
        int balance = balanceFactor(node);

        if (balance > 1) {
            // 左边高太多
            if (balanceFactor(node->left) < 0) {
                node->left = rotateLeft(node->left);
            }
            return rotateRight(node);
        }

        if (balance < -1) {
            // 右边高太多
            if (balanceFactor(node->right) > 0) {
                node->right = rotateRight(node->right);
            }
            return rotateLeft(node);
        }

        return node;
    }

    Node* insert(Node* node, const T& contact, bool& added)
    {
        // 按电话号码插入，小的放左边，大的放右边
        if (node == nullptr) {
            added = true;
            ++nodeCount;
            return new Node(contact);
        }

        const std::string phone = contact.getPhoneNumber();
        const std::string nowPhone = node->data.getPhoneNumber();
        if (phone < nowPhone) {
            node->left = insert(node->left, contact, added);
        } else if (phone > nowPhone) {
            node->right = insert(node->right, contact, added);
        } else {
            // 电话号码一样就当成同一个人，直接覆盖
            node->data = contact;
            return node;
        }

        // 回来的时候顺便把 AVL 调平
        return rebalance(node);
    }

    Node* minNode(Node* node) const
    {
        // 最小的电话号码一定一直往左走
        while (node != nullptr && node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    Node* remove(Node* node, const std::string& phoneNumber, bool& removed)
    {
        // 删除也是先按电话号码找到这个节点
        if (node == nullptr) {
            return nullptr;
        }

        if (phoneNumber < node->data.getPhoneNumber()) {
            node->left = remove(node->left, phoneNumber, removed);
        } else if (phoneNumber > node->data.getPhoneNumber()) {
            node->right = remove(node->right, phoneNumber, removed);
        } else {
            removed = true;
            if (node->left == nullptr || node->right == nullptr) {
                // 只有 0 个或 1 个儿子时，直接拿儿子顶上来
                Node* child = node->left;
                if (child == nullptr) {
                    child = node->right;
                }
                delete node;
                --nodeCount;
                return child;
            }

            // 有两个儿子时，找右子树里最小的那个来替换
            Node* successor = minNode(node->right);
            node->data = successor->data;
            bool tmpRemoved = false;
            node->right = remove(node->right, successor->data.getPhoneNumber(), tmpRemoved);
            removed = true;
        }

        // 删除完也要重新调平
        return rebalance(node);
    }

    T* find(Node* node, const std::string& phoneNumber) const
    {
        // 查找也是按电话号码往左或往右走
        if (node == nullptr) {
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
        // 中序遍历，出来就是按电话号码排好的
        if (node == nullptr) {
            return;
        }
        inorder(node->left, contacts);
        contacts.push_back(node->data);
        inorder(node->right, contacts);
    }

    void clear(Node* node)
    {
        // 释放节点，避免内存泄漏
        if (node == nullptr) {
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
        // 返回值表示有没有真的新插入，覆盖旧号码就返回 false
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
        // 外面用 vector 更方便，所以这里转一下
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
