#pragma once

template<class K>
struct BSTNode
{
    BSTNode(const K& key) :key_(key), left_(nullptr), right_(nullptr) {}
    K key_;
    BSTNode* left_;
    BSTNode* right_;
};

template<class K>
class BSTree
{
    typedef BSTNode<K> Node;
public:
    bool insert(const K& key)
    {
        Node* cur = root_;
        Node* parent = nullptr;
        while (cur) // �ҵ�����λ��parent
        {
            parent = cur;
            if (cur->key_ < key)
            {
                cur = cur->right_;
            }
            else if (cur->key_ > key)
            {
                cur = cur->left_;
            }
            else
            {
                return false;
            }
        }

        if (parent) // ����Ϊ��
        {
            // �жϲ���parent�����ң�
            if (parent->key_ < key)
            {
                parent->right_ = new Node(key);
            }
            else
            {
                parent->left_ = new Node(key);
            }
        }
        else // ��Ϊ��
        {
            root_ = new Node(key);
        }

        return true;
    }

    bool insert1(const K& key)
    {
        Node* cur = root_;
        if (cur != nullptr)
        {
            while (cur)
            {
                if (cur->key_ < key)
                {
                    if (cur->right_)
                    {
                        cur = cur->right_;
                    }
                    else
                    {
                        cur->right_ = new Node(key);
                        break;
                    }
                }
                else if (cur->key_ > key)
                {
                    if (cur->left_)
                    {
                        cur = cur->left_;
                    }
                    else
                    {
                        cur->left_ = new Node(key);
                        break;
                    }
                }
                else
                {
                    return false;
                }
            }
        }
        else
        {
            root_ = new Node(key);
        }
        return true;
    }

    void print_in_order()
    {
        print_in_order_(root_);
        std::cout << std::endl;
    }

    Node* find(const K& key)
    {
        Node* cur = root_;
        while (cur)
        {
            if (cur->key_ < key)
            {
                cur = cur->right_;
            }
            else if (cur->key_ > key)
            {
                cur = cur->left_;
            }
            else
            {
                break;
            }
        }

        return cur;
    }

    bool erase(const K& key)
    {
        Node* cur = root_;
        Node* parent = nullptr;
        while (cur)
        {
            if (cur->key_ < key)
            {
                parent = cur;
                cur = cur->right_;
            }
            else if (cur->key_ > key)
            {
                parent = cur;
                cur = cur->left_;
            }
            else
            {
                // �ҵ���ɾ���ڵ�cur

                if (cur->left_ == nullptr)
                {
                    if (cur == root_)
                    {
                        root_ = cur->right_;
                    }
                    else
                    {
                        if (cur == parent->left_)
                        {
                            parent->left_ = cur->right_;
                        }
                        else
                        {
                            parent->right_ = cur->right_;
                        }
                    }
                    delete cur;
                }
                else if (cur->right_ == nullptr)
                {
                    if (cur == root_)
                    {
                        root_ = cur->left_;
                    }
                    else
                    {
                        if (cur == parent->left_)
                        {
                            parent->left_ = cur->left_;
                        }
                        else
                        {
                            parent->right_ = cur->left_;
                        }
                    }
                    delete cur;
                }
                else // ������������Ϊ��
                {
                    // �ҵ�����������С�ڵ�
                    Node* min = cur->right_;
                    Node* min_parent = cur;
                    while (min->left_)
                    {
                        min_parent = min;
                        min = min->left_;
                    }

                    // ��С�ڵ�ֵ�滻��ɾ�ڵ�
                    std::swap(cur->key_, min->key_);

                    // ɾ������������С�ڵ�
                    if (min == min_parent->left_)
                    {
                        min_parent->left_ = min->right_;
                    }
                    else
                    {
                        min_parent->right_ = min->right_;
                    }
                    delete min;
                }

                return true;
            }
        }
        return false;
    }

    Node* find_recursion(const K& key)
    {
        return find_recursion_(root_, key);
    }

    bool insert_recursion(const K& key)
    {
        return insert_recursion_(root_, key);
    }

    bool erase_recursion(const K& key)
    {
        return erase_recursion_(root_, key);
    }
private:
    bool erase_recursion_(Node*& root, const K& key)
    {
        if (root == nullptr)
        {
            return false;
        }

        if (root->key_ < key)
        {
            return erase_recursion_(root->right_, key);
        }
        else if (root->key_ > key)
        {
            return erase_recursion_(root->left_, key);
        }
        else
        {
            // ɾ��

            if (root->left_ == nullptr)
            {
                Node* del = root;
                root = root->right_;
                delete del;
            }
            else if (root->right_ == nullptr)
            {
                Node* del = root;
                root = root->left_;
                delete del;
            }
            else
            {
                // �ҵ���ɾ�ڵ�
                Node* min = root->right_;
                while (min->left_)
                {
                    min = min->left_;
                }

                std::swap(root->key_, min->key_);

                // ת������������ɾ��
                // swap�����Ժ���������Ȼ��һ��������
                return erase_recursion_(root->right_, key);
            }

            return true;
        }
    }

    bool insert_recursion_(Node*& root, const K& key)
    {
        if (root == nullptr)
        {
            // ����
            root = new Node(key);
            return true;
        }

        if (root->key_ < key)
        {
            return insert_recursion_(root->right_, key);
        }
        else if (root->key_ > key)
        {
            return insert_recursion_(root->left_, key);
        }
        else
        {
            return false;
        }
    }

    Node* find_recursion_(Node* root, const K& key)
    {
        if (root == nullptr)
        {
            return nullptr;
        }

        if (root->key_ < key)
        {
            return find_recursion_(root->right_, key);
        }
        else if (root->key_ > key)
        {
            return find_recursion_(root->left_, key);
        }
        else
        {
            return root;
        }
    }
    void print_in_order_(const Node* root)
    {
        if (root == nullptr)
        {
            return;
        }

        print_in_order_(root->left_);
        std::cout << root->key_ << " ";
        print_in_order_(root->right_);
    }
private:
    Node* root_ = nullptr;
};

void test_binary_search_tree()
{
    int ary[] = { 33,17,50,13,18,34,58,16,25,51,66 };

    // ��֤����
    BSTree<int> t;
    for (auto e : ary)
    {
        t.insert1(e);
    }

    t.print_in_order();

    // ��֤����
    /*int key = 54;
    BSTNode<int>* target = t.find(key);
    if (target)
    {
        std::cout << "find the " << target->key_ << std::endl;
    }
    else
    {
        std::cout << "can't find the " << key << std::endl;
    }*/

    // ��֤ɾ��
    /*for (auto e : ary)
    {
        t.erase(e);
        t.print_in_order();
    }*/

    // ��֤�ݹ����
    /*int key = 51;
    BSTNode<int>* target = t.find_recursion(key);
    if (target)
    {
        std::cout << "find the " << target->key_ << std::endl;
    }
    else
    {
        std::cout << "can't find the " << key << std::endl;
    }*/

    // ��֤�ݹ����
    //for (auto e : ary)
    //{
    //    t.erase(e);
    //    //t.print_in_order();
    //}

    //for (auto e : ary)
    //{
    //    t.insert_recursion(e);
    //    t.print_in_order();
    //}

    // ��֤�ݹ�ɾ��
    for (auto e : ary)
    {
        t.erase_recursion(e);
        t.print_in_order();
    }
}