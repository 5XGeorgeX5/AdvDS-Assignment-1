#ifndef BTREE_CPP
#define BTREE_CPP

#include <iostream>

template <typename T, int N>
class BTree
{
    static_assert(N >= 3, "B-Tree must be at least in order 3");

private:
    struct Node
    {
        T data[N];
        Node *parent;
        Node *children[N + 1] = {nullptr};
        int size = 0;
        Node(Node *parent) : parent(parent) {}
        int insert(const T &value)
        {
            int index = 0;
            while (index < size && data[index] < value)
            {
                index++;
            }
            for (int i = size; i > index; i--)
            {
                data[i] = data[i - 1];
                children[i + 1] = children[i];
            }
            data[index] = value;
            size++;
            return index;
        }
    };

    Node *root = nullptr;

    Node *findNode(Node *node, const T &value)
    {
        if (node->children[0] == nullptr)
            return node;

        for (int i = 0; i < node->size; i++)
        {
            if (value < node->data[i])
                return findNode(node->children[i], value);
        }
        return findNode(node->children[node->size], value);
    }

    void fixNode(Node *node)
    {
        if (node->size < N)
            return;

        Node *parent = node->parent;
        if (parent == nullptr)
        {
            parent = new Node(nullptr);
            root = parent;
        }
        node->parent = parent;
        Node *newRightNode = new Node(parent);
        int midIndex = (N - 1) / 2;
        for (int i = midIndex + 1; i < N; i++)
        {
            newRightNode->data[i - midIndex - 1] = node->data[i];
            newRightNode->children[i - midIndex - 1] = node->children[i];
            if (node->children[i] != nullptr)
                node->children[i]->parent = newRightNode;
        }
        newRightNode->children[N - midIndex - 1] = node->children[N];
        if (node->children[N] != nullptr)
            node->children[N]->parent = newRightNode;

        newRightNode->size = N - midIndex - 1;
        int parentIndex = parent->insert(node->data[midIndex]);
        parent->children[parentIndex] = node;
        parent->children[parentIndex + 1] = newRightNode;
        node->size = midIndex;
        fixNode(parent);
    }

    void print(Node *node, int indentation)
    {
        if (node == nullptr)
            return;

        std::cout << std::string(indentation, ' ');
        std::cout << node->data[0];
        for (int i = 1; i < node->size; i++)
            std::cout << ',' << node->data[i];

        std::cout << '\n';

        for (int i = 0; i <= node->size; i++)
            print(node->children[i], indentation + 2);
    }

public:
    BTree() {};
    void Insert(const T &value)
    {
        if (root == nullptr)
        {
            root = new Node(nullptr);
            root->insert(value);
            return;
        }
        Node *node = findNode(root, value);
        node->insert(value);
        fixNode(node);
    }
    void Print()
    {
        print(root, 0);
    }
};

#endif