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
        while (cur) // 找到插入位置parent
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

        if (parent) // 根不为空
        {
            // 判断插入parent的左右？
            if (parent->key_ < key)
            {
                parent->right_ = new Node(key);
            }
            else
            {
                parent->left_ = new Node(key);
            }
        }
        else // 根为空
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
                // 找到待删除节点cur

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
                else // 左右子树都不为空
                {
                    // 找到右子树中最小节点
                    Node* min = cur->right_;
                    Node* min_parent = cur;
                    while (min->left_)
                    {
                        min_parent = min;
                        min = min->left_;
                    }

                    // 最小节点值替换待删节点
                    std::swap(cur->key_, min->key_);

                    // 删除右子树中最小节点
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

    void destroy()
    {
        destroy_(root_);
    }
private:
    void destroy_(Node*& root)
    {
        if (root == nullptr)
        {
            return;
        }

        destroy_(root->left_);
        destroy_(root->right_);
        delete root;
        root = nullptr;
    }

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
            // 删除

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
                // 找到待删节点
                Node* min = root->right_;
                while (min->left_)
                {
                    min = min->left_;
                }

                std::swap(root->key_, min->key_);

                // 转换成在子树中删除
                // swap交换以后，其子树依然是一颗搜索树
                return erase_recursion_(root->right_, key);
            }

            return true;
        }
    }

    bool insert_recursion_(Node*& root, const K& key)
    {
        if (root == nullptr)
        {
            // 插入
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

    // 验证插入
    BSTree<int> t;
    for (auto e : ary)
    {
        t.insert1(e);
    }

    t.print_in_order();

    // 验证查找
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

    // 验证删除
    /*for (auto e : ary)
    {
        t.erase(e);
        t.print_in_order();
    }*/

    // 验证递归查找
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

    // 验证递归插入
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

    // 验证递归删除
    for (auto e : ary)
    {
        t.erase_recursion(e);
        t.print_in_order();
    }
}

namespace key_value
{
    template<class K, class V>
    struct BSTNode
    {
        BSTNode(const K& key, const V& value) :
            key_(key), value_(value), left_(nullptr), right_(nullptr) {}
        K key_;
        V value_;
        BSTNode* left_;
        BSTNode* right_;
    };

    template<class K, class V>
    class BSTree
    {
        typedef BSTNode<K, V> Node;
    public:
        bool insert(const K& key, const V& value)
        {
            Node* cur = root_;
            Node* parent = nullptr;
            while (cur) // 找到插入位置parent
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

            if (parent) // 根不为空
            {
                // 判断插入parent的左右？
                if (parent->key_ < key)
                {
                    parent->right_ = new Node(key, value);
                }
                else
                {
                    parent->left_ = new Node(key, value);
                }
            }
            else // 根为空
            {
                root_ = new Node(key, value);
            }

            return true;
        }

        bool insert1(const K& key, const V& value)
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
                            cur->right_ = new Node(key, value);
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
                            cur->left_ = new Node(key, value);
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
                root_ = new Node(key, value);
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
                    // 找到待删除节点cur

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
                    else // 左右子树都不为空
                    {
                        // 找到右子树中最小节点
                        Node* min = cur->right_;
                        Node* min_parent = cur;
                        while (min->left_)
                        {
                            min_parent = min;
                            min = min->left_;
                        }

                        // 最小节点值替换待删节点
                        std::swap(cur->key_, min->key_);
                        std::swap(cur->value_, min->value_);

                        // 删除右子树中最小节点
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

        bool insert_recursion(const K& key, const V& value)
        {
            return insert_recursion_(root_, key, value);
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
                // 删除

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
                    // 找到待删节点
                    Node* min = root->right_;
                    while (min->left_)
                    {
                        min = min->left_;
                    }

                    std::swap(root->key_, min->key_);
                    std::swap(root->value_, root->value_);

                    // 转换成在子树中删除
                    // swap交换以后，其子树依然是一颗搜索树
                    return erase_recursion_(root->right_, key);
                }

                return true;
            }
        }

        bool insert_recursion_(Node*& root, const K& key, const V& value)
        {
            if (root == nullptr)
            {
                // 插入
                root = new Node(key, value);
                return true;
            }

            if (root->key_ < key)
            {
                return insert_recursion_(root->right_, key, value);
            }
            else if (root->key_ > key)
            {
                return insert_recursion_(root->left_, key, value);
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
            std::cout << root->key_ << ":" << root->value_ << " ";
            print_in_order_(root->right_);
        }
    private:
        Node* root_ = nullptr;
    };

    void test_binary_search_tree_kv()
    {
        BSTree<std::string, int> dict;
        dict.insert("One", 1);
        dict.insert("Two", 2);
        dict.insert("Three", 3);
        dict.insert("Four", 4);
        dict.insert("Five", 5);

        dict.print_in_order();
    }
}

namespace mypair
{
    template<class K, class V>
    struct BSTNode
    {
        BSTNode(const std::pair<K, V> new_pair) :
            pair_(new_pair), left_(nullptr), right_(nullptr) {}
        std::pair<K, V> pair_;
        BSTNode* left_;
        BSTNode* right_;
    };

    template<class K, class V>
    class BSTree
    {
        typedef K key_type;
        typedef V value_type;
        typedef BSTNode<key_type, value_type> Node;

    public:
        bool insert(const std::pair<key_type, value_type>& new_pair)
        {
            const key_type& key = new_pair.first;
            const value_type& value = new_pair.second;

            Node* cur = root_;
            Node* parent = nullptr;
            while (cur) // 找到插入位置parent
            {
                parent = cur;
                if (cur->pair_.first < key)
                {
                    cur = cur->right_;
                }
                else if (cur->pair_.first > key)
                {
                    cur = cur->left_;
                }
                else
                {
                    return false;
                }
            }

            if (parent) // 根不为空
            {
                // 判断插入parent的左右？
                if (parent->pair_.first < key)
                {
                    parent->right_ = new Node(new_pair);
                }
                else
                {
                    parent->left_ = new Node(new_pair);
                }
            }
            else // 根为空
            {
                root_ = new Node(new_pair);
            }

            return true;
        }

        void print_in_order()
        {
            print_in_order_(root_);
            std::cout << std::endl;
        }

        Node* find(const key_type& key)
        {
            Node* cur = root_;
            while (cur)
            {
                if (cur->pair_.first < key)
                {
                    cur = cur->right_;
                }
                else if (cur->pair_.first > key)
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

        bool erase(const key_type& key)
        {
            Node* cur = root_;
            Node* parent = nullptr;
            while (cur)
            {
                if (cur->pair_.first < key)
                {
                    parent = cur;
                    cur = cur->right_;
                }
                else if (cur->pair_.first > key)
                {
                    parent = cur;
                    cur = cur->left_;
                }
                else
                {
                    // 找到待删除节点cur

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
                    else // 左右子树都不为空
                    {
                        // 找到右子树中最小节点
                        Node* min = cur->right_;
                        Node* min_parent = cur;
                        while (min->left_)
                        {
                            min_parent = min;
                            min = min->left_;
                        }

                        // 最小节点值替换待删节点
                        std::swap(cur->pair_, min->pair_);

                        // 删除右子树中最小节点
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

    private:
        void print_in_order_(const Node* root)
        {
            if (root == nullptr)
            {
                return;
            }

            print_in_order_(root->left_);
            std::cout << root->pair_.first << ":" << root->pair_.second << " ";
            print_in_order_(root->right_);
        }
    private:
        Node* root_ = nullptr;
    };

    void test_binary_search_tree_kv()
    {
        BSTree<std::string, int> dict;
        dict.insert(std::pair<std::string, int>("One", 1));
        dict.insert(std::make_pair("Two", 2));
        dict.insert(std::make_pair("Three", 3));
        dict.insert(std::make_pair("Four", 4));
        dict.insert(std::make_pair("Five", 5));

        dict.print_in_order();
    }
}