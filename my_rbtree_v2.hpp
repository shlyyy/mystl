#pragma once

enum Color
{
    RED,
    BLACK
};

template<class T>
struct RBTreeNode
{
    RBTreeNode* left_;
    RBTreeNode* right_;
    RBTreeNode* parent_;
    T data_; // value_type: for set is key, for map is pair<const key, value>
    Color col_;

    RBTreeNode(const T& data)
        :left_(nullptr), right_(nullptr), parent_(nullptr), data_(data), col_(RED)
    {}
};

template<class T, class Ref, class Ptr>
struct RBTreeIterator
{
    typedef RBTreeNode<T> Node;
    typedef RBTreeIterator<T, Ref, Ptr> Self;

    Node* node_; // ������ڵ��ָ��

    RBTreeIterator(Node* node) : node_(node) {}

    // ��ӹ��캯��������RBTree���صĵ���������
    template<typename U, typename V, typename W>
    RBTreeIterator(const RBTreeIterator<U, V, W>& other) : node_(other.node_) {}

    Ref operator*()
    {
        return node_->data_;
    }

    Ptr operator->()
    {
        return &node_->data_;
    }

    bool operator!=(const Self& s)
    {
        return node_ != s.node_;
    }

    Self& operator++()
    {
        if (node_->right_)
        {
            // ��һ������������������ڵ�
            Node* cur = node_->right_;
            while (cur->left_)
            {
                cur = cur->left_;
            }
            node_ = cur;
        }
        else
        {
            // ��һ������cur��parent��ڵ�ʱ����Ӧ��parent�ڵ�
            Node* cur = node_;
            Node* parent = cur->parent_;
            while (parent && cur == parent->right_)
            {
                cur = parent;
                parent = cur->parent_;
            }
            node_ = parent;
        }

        return *this;
    }
};

template<class K, class T, class KeyOfT>
class RBTree
{
    typedef RBTreeNode<T> Node;
public:
    typedef RBTreeIterator<T, T&, T*> iterator;
    typedef RBTreeIterator<T, const T&, const T*> const_iterator;

public:
    std::pair<iterator, bool> insert(const T& data)
    {
        if (root_ == nullptr)
        {
            root_ = new Node(data);
            root_->col_ = BLACK;
            return std::make_pair(root_, true);
        }

        KeyOfT  kot;

        Node* cur = root_;
        Node* parent = nullptr;
        while (cur)
        {
            parent = cur;
            if (kot(cur->data_) < kot(data))
            {
                cur = cur->right_;
            }
            else if (kot(cur->data_) > kot(data))
            {
                cur = cur->left_;
            }
            else
            {
                return std::make_pair(cur, false);
            }
        }

        cur = new Node(data);
        Node* new_node = cur;
        if (kot(parent->data_) < kot(data))
        {
            parent->right_ = cur;
        }
        else
        {
            parent->left_ = cur;
        }
        cur->parent_ = parent;

        // ���ڵ�Ϊ��ɫ��Ҫƽ�����
        while (parent && parent->col_ == RED)
        {
            Node* grandpa = parent->parent_;
            if (parent == grandpa->left_)
            {
                // p��g����ڵ�
                //     g
                //   p   u
                // c

                Node* uncle = grandpa->right_;
                if (uncle && uncle->col_ == RED) // ���������Ϊ�� --> ��ɫ
                {
                    // ���׺������� үү���
                    parent->col_ = uncle->col_ = BLACK;
                    grandpa->col_ = RED;

                    // үү���֮����ܻ���Ҫ�������ϵ���
                    // ���grandpa�Ǹ�����parentΪ�ս���ѭ����
                    // ��ʱgrandpa����Ҫ�ú�������whileѭ��������ֱ���úڸ��ڵ�
                    cur = grandpa;
                    parent = cur->parent_;
                }
                else // ���岻���ڣ��������ڵ�Ϊ�� --> ��ת + ��ɫ
                {
                    if (cur == parent->left_)
                    {
                        // p��g����ڵ�
                        //       g
                        //   p       u
                        //  c

                        // �������� + ��ɫ
                        rotate_right(grandpa);
                        parent->col_ = BLACK;
                        grandpa->col_ = RED;
                    }
                    else
                    {
                        // p��g����ڵ�
                        //       g
                        //   p       u
                        //    c

                        // ���ң����� + ���� + ��ɫ
                        rotate_left(parent);
                        rotate_right(grandpa);
                        cur->col_ = BLACK;
                        grandpa->col_ = RED;
                    }

                    // ��������
                    break;
                }
            }
            else // parent == grandpa->right_
            {
                // p��g���ҽڵ�
                //     g
                //   u   p
                //         c

                Node* uncle = grandpa->left_;
                if (uncle && uncle->col_ == RED) // ���������Ϊ�� --> ��ɫ
                {
                    parent->col_ = uncle->col_ = BLACK;
                    grandpa->col_ = RED;

                    cur = grandpa;
                    parent = cur->parent_;
                }
                else // ���岻���ڣ��������ڵ�Ϊ�� --> ��ת + ��ɫ
                {
                    if (cur == parent->right_)
                    {
                        // p��g���ҽڵ�
                        //       g
                        //   u       p
                        //             c

                        // ���ң����� + ��ɫ
                        rotate_left(grandpa);
                        parent->col_ = BLACK;
                        grandpa->col_ = RED;
                    }
                    else
                    {
                        // p��g���ҽڵ�
                        //       g
                        //   u       p
                        //         c

                        // �������� + ���� + ��ɫ
                        rotate_right(parent);
                        rotate_left(grandpa);
                        cur->col_ = BLACK;
                        grandpa->col_ = RED;
                    }

                    // ��������
                    break;
                }
            }
        }

        root_->col_ = BLACK; // �����Ǻ첢�ҵ�������

        return std::make_pair(new_node, true);
    }

    void rotate_left(Node* parent)
    {
        Node* subR = parent->right_;
        Node* subRL = subR->left_;

        parent->right_ = subRL;
        subR->left_ = parent;

        Node* parentParent = parent->parent_;

        parent->parent_ = subR;
        if (subRL)
            subRL->parent_ = parent;

        if (root_ == parent)
        {
            root_ = subR;
            subR->parent_ = nullptr;
        }
        else
        {
            if (parentParent->left_ == parent)
            {
                parentParent->left_ = subR;
            }
            else
            {
                parentParent->right_ = subR;
            }

            subR->parent_ = parentParent;
        }
    }

    void rotate_right(Node* parent)
    {
        Node* subL = parent->left_;
        Node* subLR = subL->right_;

        parent->left_ = subLR;
        if (subLR)
            subLR->parent_ = parent;

        Node* parentParent = parent->parent_;

        subL->right_ = parent;
        parent->parent_ = subL;

        if (root_ == parent)
        {
            root_ = subL;
            subL->parent_ = nullptr;
        }
        else
        {
            if (parentParent->left_ == parent)
            {
                parentParent->left_ = subL;
            }
            else
            {
                parentParent->right_ = subL;
            }

            subL->parent_ = parentParent;
        }
    }

    void print_in_order()
    {
        print_in_order_(root_);
        std::cout << std::endl;
    }

    bool balance()
    {
        if (root_ == nullptr)
            return true;

        if (root_->col_ == RED)
            return false;

        // �ο�ֵ
        int comp_value = 0;
        Node* cur = root_;
        while (cur)
        {
            if (cur->col_ == BLACK)
            {
                ++comp_value;
            }

            cur = cur->left_;
        }

        int blacknum = 0;
        return valid_(root_, blacknum, comp_value);
    }

    int height()
    {
        return height_(root_);
    }

    size_t size()
    {
        return size_(root_);
    }

    Node* find(const K& key)
    {
        Node* cur = root_;
        while (cur)
        {
            if (cur->kv_.first < key)
            {
                cur = cur->right_;
            }
            else if (cur->kv_.first > key)
            {
                cur = cur->left_;
            }
            else
            {
                return cur;
            }
        }

        return nullptr;
    }

    iterator begin()
    {
        Node* cur = root_;
        while (cur && cur->left_)
        {
            cur = cur->left_;
        }

        return iterator(cur);
    }

    iterator end()
    {
        return iterator(nullptr);
    }

    const_iterator begin() const
    {
        Node* cur = root_;
        while (cur && cur->left_)
        {
            cur = cur->left_;
        }

        return const_iterator(cur);
    }

    const_iterator end() const
    {
        return const_iterator(nullptr);
    }

    // Function to print the tree
    void print_tree() {
        print_tree_helper_(root_, 0);
    }
private:
    // Helper function to print the tree
    void print_tree_helper_(Node* root, int space) {
        if (root != nullptr) {
            space += 10;
            print_tree_helper_(root->right_, space);
            std::cout << std::endl;
            for (int i = 10; i < space; i++)
                std::cout << " ";
            char color = root->col_ == RED ? 'r' : 'b';
            std::cout << root->kv_.first << ":" << color << std::endl;
            print_tree_helper_(root->left_, space);
        }
    }

private:
    // blacknum:�Ӹ��ڵ�->��ǰ�ڵ��·���к�ɫ�ڵ������
    bool valid_(Node* root, int blacknum, const int comp_value)
    {
        if (root == nullptr)
        {
            if (blacknum != comp_value)
            {
                std::cout << "���ں�ɫ�ڵ���������ȵ�·��" << std::endl;
                return false;
            }

            return true;
        }

        if (root->col_ == RED && root->parent_->col_ == RED)
        {
            std::cout << "�������ĺ�ɫ�ڵ�" << std::endl;

            return false;
        }

        if (root->col_ == BLACK)
        {
            ++blacknum;
        }

        return valid_(root->left_, blacknum, comp_value)
            && valid_(root->right_, blacknum, comp_value);
    }

    size_t size_(Node* root)
    {
        if (root == nullptr)
            return 0;

        return size_(root->left_) + size_(root->right_) + 1;
    }

    int height_(Node* root)
    {
        if (root == nullptr)
            return 0;

        int leftHeight = height_(root->left_);
        int rightHeight = height_(root->right_);

        return leftHeight > rightHeight ? leftHeight + 1 : rightHeight + 1;
    }

    void print_in_order_(Node* root)
    {
        if (root == nullptr)
            return;

        print_in_order_(root->left_);
        std::cout << root->kv_.first << " ";
        print_in_order_(root->right_);
    }

protected:
    Node* root_ = nullptr;
};

//void test_rb_tree()
//{
//    //const int N = 1000000;
//    const int N = 10;
//    std::vector<int> v;
//    v.reserve(N);
//    //srand(time(0));
//
//    for (size_t i = 0; i < N; i++)
//    {
//        //v.push_back(rand() * 100 + rand() * 10 + rand());
//        v.push_back(rand());
//    }
//
//    RBTree<int, int> t;
//
//    for (size_t i = 0; i < N; i++)
//    {
//        t.insert(std::make_pair(v[i], v[i]));
//    }
//
//    //t.print_in_order();
//
//    t.print_tree();
//
//    std::cout << "size=" << t.size() << std::endl;
//    std::cout << t.balance() << std::endl;
//    std::cout << t.height() << std::endl;
//}