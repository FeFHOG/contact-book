#ifndef CONTACT_TREE_H
#define CONTACT_TREE_H

#include <algorithm>
#include <string>
#include <vector>

template <typename T>
class ContactTree {
private:
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

    int height(Node* node) const
    {
        return node ? node->height : 0;
    }

    int balanceFactor(Node* node) const
    {
        return node ? height(node->left) - height(node->right) : 0;
    }

    void updateHeight(Node* node)
    {
        if (node) {
            node->height = std::max(height(node->left), height(node->right)) + 1;
        }
    }

    Node* rotateRight(Node* y)
    {
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
        updateHeight(node);
        int balance = balanceFactor(node);

        if (balance > 1) {
            if (balanceFactor(node->left) < 0) {
                node->left = rotateLeft(node->left);
            }
            return rotateRight(node);
        }

        if (balance < -1) {
            if (balanceFactor(node->right) > 0) {
                node->right = rotateRight(node->right);
            }
            return rotateLeft(node);
        }

        return node;
    }

    Node* insert(Node* node, const T& contact, bool& added)
    {
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
            node->data = contact;
            return node;
        }

        return rebalance(node);
    }

    Node* minNode(Node* node) const
    {
        while (node && node->left) {
            node = node->left;
        }
        return node;
    }

    Node* remove(Node* node, const std::string& phoneNumber, bool& removed)
    {
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
                Node* child = node->left ? node->left : node->right;
                delete node;
                --nodeCount;
                return child;
            }

            Node* successor = minNode(node->right);
            node->data = successor->data;
            bool removedSuccessor = false;
            node->right = remove(node->right, successor->data.getPhoneNumber(), removedSuccessor);
            removed = true;
        }

        return rebalance(node);
    }

    T* find(Node* node, const std::string& phoneNumber) const
    {
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
        if (!node) {
            return;
        }
        inorder(node->left, contacts);
        contacts.push_back(node->data);
        inorder(node->right, contacts);
    }

    void clear(Node* node)
    {
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
