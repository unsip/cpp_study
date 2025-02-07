#pragma once

#include <cmath>


template <typename T, typename Comparator>
class BinaryTree
{
    struct Node
    {
        T data;
        Node* left = nullptr, * right, * parent = nullptr;
    };

    Node* m_root = nullptr;
    std::size_t m_sz = 0;
    Comparator cmp;

public:
    BinaryTree();
    ~BinaryTree();

    Node* begin()
    {
        Node* curr = root;
        while (curr && curr->left)
            curr = curr->left;

        return curr;
    }

    Node* end()
    {
        Node* curr = root;
        while (curr && curr->right)
            curr = curr->right;

        return curr;
    }

    Node* insert(T elem)
    {
        auto p = find(elem);
        if (!p.first)
        {
            Node* n = new Node(elem);
            n.parent = p.second;
            ++m_sz;
            if (!p.second)
                m_root = n;
            else if (m_cmp(p.second->data, elem))
                p.second->left = n;
            else
                p.second->right = n;

            return n;
        }

        return p.first;
    }

    Node* erase(T elem)
    {
        auto p = find(elem);
        return p.second;
    }

    std::pair<Node*, Node*> find(T elem)
    {
        Node* it = root;
        Node* parent = nullptr;
        while (it)
        {
            parent = it;
            if (m_cmp(it->data, elem))
                it = it->right;
            else if (m_cmp(elem, it->data))
                it = it->left;
            else
                break;
        }

        return std::pair<Node*, Node*>(it, parent);
    }

    bool empty() { return m_sz == 0; }
    std::size_t size() { return m_sz; }


    // @todo: Implement
    Node* next(Node* it)
    {
        return it->;
    }
};

