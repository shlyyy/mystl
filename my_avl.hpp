#pragma once

template<class K, class V>
struct AVLNode
{
    AVLNode(const std::pair<K, V>& kv) :
        kv_(kv), bf_(0), parent_(nullptr), left_(nullptr), right_(nullptr)
    {}

    AVLNode* parent_;
    AVLNode* left_;
    AVLNode* right_;
    std::pair<K, V> kv_;
    int bf_; // balance factor
};

template<class K, class V>
class AVLTree
{
    typedef AVLNode<K, V> Node;

public:
    std::pair<Node*, bool> insert(const std::pair<K, V>& kv)
    {
        if (root_ == nullptr)
        {
            root_ = new Node(kv);
            return std::make_pair(root_, true);
        }

        Node* parent = nullptr;
        Node* cur = root_;

        while (cur)
        {
            if (cur->kv_.first < kv.first)
            {
                parent = cur;
                cur = cur->right_;
            }
            else if (cur->kv_.first > kv.first)
            {
                parent = cur;
                cur = cur->left_;
            }
            else
            {
                return std::make_pair(nullptr, false);
            }
        }

        cur = new Node(kv);
        Node* ret = cur;
        if (parent->kv_.first < kv.first)
        {
            parent->right_ = cur;
        }
        else
        {
            parent->left_ = cur;
        }
        cur->parent_ = parent;

        // 平衡调整
        while (parent)
        {
            if (cur == parent->left_)
            {
                parent->bf_--;
            }
            else
            {
                parent->bf_++;
            }

            if (parent->bf_ == 0)
            {
                break;
            }
            else if (parent->bf_ == 1 || parent->bf_ == -1)
            {
                cur = parent;
                parent = parent->parent_;
            }
            else if (parent->bf_ == 2 || parent->bf_ == -2)
            {
                if (parent->bf_ == 2 && cur->bf_ == 1)
                {
                    rotate_left(parent);
                }
                else if (parent->bf_ == -2 && cur->bf_ == -1)
                {
                    rotate_right(parent);
                }
                else if (parent->bf_ == 2 && cur->bf_ == -1)
                {
                    rotate_right_left(parent);
                }
                else if (parent->bf_ == -2 && cur->bf_ == 1)
                {
                    rotate_left_right(parent);
                }

                // 1、旋转让这颗子树平衡了
                // 2、旋转降低了这颗子树的高度，恢复到跟插入前一样的高度，所以对上一层没有影响，不用继续更新
                break;
            }
            else
            {
                assert(false);
            }
        }

        return std::make_pair(ret, true);
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

        parent->bf_ = subR->bf_ = 0;
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

        subL->bf_ = parent->bf_ = 0;
    }

    void rotate_right_left(Node* parent)
    {
        Node* subR = parent->right_;
        Node* subRL = subR->left_;
        int bf = subRL->bf_;

        rotate_right(parent->right_);
        rotate_left(parent);

        if (bf == 0)
        {
            // subRL自己就是新增
            parent->bf_ = subR->bf_ = subRL->bf_ = 0;
        }
        else if (bf == -1)
        {
            // subRL的左子树新增
            parent->bf_ = 0;
            subRL->bf_ = 0;
            subR->bf_ = 1;
        }
        else if (bf == 1)
        {
            // subRL的右子树新增
            parent->bf_ = -1;
            subRL->bf_ = 0;
            subR->bf_ = 0;
        }
        else
        {
            assert(false);
        }
    }

    void rotate_left_right(Node* parent)
    {
        Node* subL = parent->left_;
        Node* subLR = subL->right_;
        int bf = subLR->bf_;

        rotate_left(parent->left_);
        rotate_right(parent);

        if (bf == 1)
        {
            parent->bf_ = 0;
            subLR->bf_ = 0;
            subL->bf_ = -1;
        }
        else if (bf == -1)
        {
            parent->bf_ = 1;
            subLR->bf_ = 0;
            subL->bf_ = 0;
        }
        else if (bf == 0)
        {
            parent->bf_ = 0;
            subLR->bf_ = 0;
            subL->bf_ = 0;
        }
        else
        {
            assert(false);
        }
    }

    void print_in_order()
    {
        print_in_order_(root_);
        std::cout << std::endl;
    }

    bool balance()
    {
        return balance_(root_);
    }

    int height()
    {
        return height_(root_);
    }

    size_t size()
    {
        return size_(root_);
    }

private:
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
        std::cout << root->_kv.first << " ";
        print_in_order_(root->right_);
    }

    bool balance_(Node* root)
    {
        if (root == nullptr)
            return true;

        int left_height = height_(root->left_);
        int right_height = height_(root->right_);
        if (right_height - left_height != root->bf_)
        {
            std::cout << root->kv_.first << "平衡因子异常" << std::endl;
            return false;
        }

        return abs(right_height - left_height) < 2
            && balance_(root->left_)
            && balance_(root->right_);
    }

private:
    Node* root_ = nullptr;
};

void test_avl_tree()
{
    const int N = 1000000;
    std::vector<int> v;
    v.reserve(N);
    //srand(time(0));

    for (size_t i = 0; i < N; i++)
    {
        v.push_back(rand());
        //std::cout << v.back() << std::endl;
    }

    AVLTree<int, int> t;

    size_t count = 0;
    for (size_t i = 0; i < N; i++)
    {
        std::pair<AVLNode<int, int>*, bool> ret = t.insert(std::make_pair(v[i], v[i]));
        if (ret.second)
        {
            if (i % 1000 == 0)
            {
                std::cout << "i=" << i << " height:" << t.height() << " ";
                std::cout << "Insert:" << v[i] << "->" << t.balance() << std::endl;
            }
        }
        else
        {
            ++count;
        }
    }

    std::cout << count << " size=" << t.size() << std::endl;
    std::cout << t.balance() << std::endl;
    std::cout << t.height() << std::endl;
}