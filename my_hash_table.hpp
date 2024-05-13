#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>

template<class K>
class HashFuncs
{
public:
    size_t operator()(const K& key)
    {
        return (size_t)key;
    }
};

// 各种字符串Hash函数：https://www.cnblogs.com/-clq/archive/2012/05/31/2528153.html
template<>
class HashFuncs<std::string>
{
public:
    size_t operator()(const std::string& key)
    {
        // BKDR
        size_t hash = 0;
        for (auto e : key)
        {
            hash = hash * 131 + e;
        }

        //std::cout << key << ":" << hash << std::endl;
        return hash;
    }
};

namespace open_addressing_hashing
{
    enum Status
    {
        EMPTY,
        EXIST,
        DELETE
    };

    template<class K, class V>
    struct HashNode
    {
        std::pair<K, V> kv_;
        Status status_;
    };

    template<class K, class V, class HashFunc = HashFuncs<K>>
    class HashTable
    {
    public:
        HashTable()
        {
            tables_.resize(10);
        }

        bool insert(const std::pair<K, V>& kv)
        {
            // 1.重复元素不插入
            if (find(kv.first))
                return false;

            // 2.负载因子为0.7时就扩容
            if (num_elems_ * 10 / tables_.size() >= 7)
            {
                size_t new_size = tables_.size() * 2;
                HashTable<K, V> new_tables;
                new_tables.tables_.resize(new_size);

                // 遍历旧表重新散列
                for (size_t i = 0; i < tables_.size(); ++i)
                {
                    if (tables_[i].status_ == EXIST)
                    {
                        new_tables.insert(tables_[i].kv_);
                    }
                }

                tables_.swap(new_tables.tables_);
                // num_elems_个数相同无需交换
            }

            // 3.线性探测插入数据
            size_t hashing_index = hash_func_(kv.first) % tables_.size();
            while (tables_[hashing_index].status_ == EXIST) // 找到插入位置
            {
                ++hashing_index;
                hashing_index %= tables_.size();
            }

            // 插入数据
            tables_[hashing_index].kv_ = kv;
            tables_[hashing_index].status_ = EXIST;
            ++num_elems_;

            return true;
        }

        HashNode<K, V>* find(const K& key)
        {
            size_t hashing_index = hash_func_(key) % tables_.size();
            while (tables_[hashing_index].status_ != EMPTY)
            {
                if (tables_[hashing_index].status_ == EXIST // 插入x(E)，删除x(D)，再插入x(Find)
                    && tables_[hashing_index].kv_.first == key)
                {
                    return &tables_[hashing_index];
                }
                ++hashing_index;
                hashing_index %= tables_.size();
            }

            return nullptr;
        }

        bool erase(const K& key)
        {
            HashNode<K, V>* ret = find(key);
            if (ret)
            {
                ret->status_ = DELETE;
                --num_elems_;
                return true;
            }
            return false;
        }

        void print()
        {
            for (size_t i = 0; i < tables_.size(); i++)
            {
                if (tables_[i].status_ == EXIST)
                {
                    std::cout << "[" << i << "]->"
                        << tables_[i].kv_.first << ":"
                        << tables_[i].kv_.second << std::endl;
                }
                else if (tables_[i].status_ == EMPTY)
                {
                    std::cout << "[" << i << "]->" << std::endl;
                }
                else
                {
                    std::cout << "[" << i << "]->" << "D" << std::endl;
                }
            }

            std::cout << std::endl;
        }

    private:
        std::vector<HashNode<K, V>> tables_;
        size_t num_elems_ = 0; // 存储关键字的个数
        HashFunc hash_func_;
    };

    void test_hash()
    {
        std::string arr[] = { "香蕉", "甜瓜","苹果", "西瓜", "苹果", "西瓜",
            "苹果", "苹果", "西瓜", "苹果", "香蕉", "苹果", "香蕉" };

        HashTable<std::string, int> ht;
        for (auto& e : arr)
        {
            //auto ret = ht.Find(e);
            HashNode<std::string, int>* ret = ht.find(e);
            if (ret)
            {
                ret->kv_.second++;
            }
            else
            {
                ht.insert(std::make_pair(e, 1));
            }
        }

        ht.print();

        ht.insert(std::make_pair("apple", 1));
        ht.insert(std::make_pair("sort", 1));

        ht.insert(std::make_pair("abc", 1));
        ht.insert(std::make_pair("acb", 1));
        ht.insert(std::make_pair("aad", 1));

        ht.print();
    }
}

namespace chained_hashing
{
    template<class K, class V>
    struct HashNode
    {
        HashNode* next_;
        std::pair<K, V> kv_;

        HashNode(const std::pair<K, V>& kv)
            : kv_(kv), next_(nullptr)
        {}
    };

    template<class K, class V, class HashFunc = HashFuncs<K>>
    class HashTable
    {
        typedef HashNode<K, V> Node;

    public:
        HashTable()
        {
            tables_.resize(10);
        }

        ~HashTable()
        {
            for (size_t i = 0; i < tables_.size(); ++i)
            {
                Node* cur = tables_[i];
                while (cur)
                {
                    Node* next = cur->next_;
                    delete cur;
                    cur = next;
                }
                tables_[i] = nullptr;
            }
        }

        bool insert(const std::pair<K, V>& kv)
        {
            // 1.重复元素不插入
            if (find(kv.first))
                return false;

            // 2.负载因子0.7就扩容
            //if (num_elems_ == tables_.size())
            if (num_elems_ * 10 / tables_.size() >= 7)
            {
                std::vector<Node*> new_tables;
                new_tables.resize(tables_.size() * 2, nullptr);

                // 遍历旧表将其节点重新散列到新表中
                for (size_t i = 0; i < tables_.size(); ++i)
                {
                    Node* cur = tables_[i];
                    while (cur)
                    {
                        Node* next = cur->next_;

                        // 散列到新表中 头插法
                        size_t hashing_index = hash_func_(cur->kv_.first) % new_tables.size();
                        cur->next_ = new_tables[hashing_index];
                        new_tables[hashing_index] = cur;

                        cur = next;
                    }
                    tables_[i] = nullptr;
                }

                tables_.swap(new_tables);
            }

            // 3.头插法插入元素
            size_t hashing_index = hash_func_(kv.first) % tables_.size();
            Node* new_node = new Node(kv);
            new_node->next_ = tables_[hashing_index];
            tables_[hashing_index] = new_node;
            ++num_elems_;

            return true;
        }

        Node* find(const K& key)
        {
            size_t hashing_index = hash_func_(key) % tables_.size();
            Node* cur = tables_[hashing_index];
            while (cur)
            {
                if (cur->kv_.first == key)
                {
                    return cur;
                }
                cur = cur->next_;
            }

            return nullptr;
        }

        bool erase(const K& key)
        {
            size_t hashing_index = hash_func_(key) % tables_.size();
            Node* cur = tables_[hashing_index];
            Node* prev = nullptr;
            while (cur)
            {
                if (cur->kv_.first == key)
                {
                    if (prev == nullptr)
                    {
                        tables_[hashing_index] = cur->next_;
                    }
                    else
                    {
                        prev->next_ = cur->next_;
                    }
                    delete cur;

                    --num_elems_;

                    return true;
                }

                prev = cur;
                cur = cur->next_;
            }

            return false;
        }

        void print_info()
        {
            size_t bucket_size = 0; // 桶的个数
            size_t max_bucket_len = 0; // 桶中最大元素个数

            for (size_t i = 0; i < tables_.size(); i++)
            {
                Node* cur = tables_[i];
                if (cur)
                {
                    ++bucket_size;
                }

                size_t bucket_len = 0;
                while (cur)
                {
                    ++bucket_len;
                    cur = cur->next_;
                }

                if (bucket_len > max_bucket_len)
                {
                    max_bucket_len = bucket_len;
                }
            }

            double average_bucket_len = 0; // 平均每个桶中的元素个数
            average_bucket_len = num_elems_ / (double)bucket_size;

            printf("all bucket_size:%zu\n", tables_.size());
            printf("num_elems_ size:%zu\n", num_elems_);
            printf("used bucket_size:%zu\n", bucket_size);
            printf("max_bucket_len:%zu\n", max_bucket_len);
            printf("average_bucket_len:%lf\n\n", average_bucket_len);
        }

    private:
        std::vector<Node*> tables_;
        size_t num_elems_ = 0;
        HashFunc hash_func_;
    };

    void test_hash()
    {
        std::vector<std::string> arr = { "香蕉", "甜瓜","苹果", "西瓜", "苹果", "西瓜",
            "苹果", "苹果", "西瓜", "苹果", "香蕉", "苹果", "香蕉" };

        HashTable<std::string, int> ht;
        for (auto& e : arr)
        {
            //auto ret = ht.Find(e);
            HashNode<std::string, int>* ret = ht.find(e);
            if (ret)
            {
                ret->kv_.second++;
            }
            else
            {
                ht.insert(std::make_pair(e, 1));
            }
        }

        std::set<std::string> s(arr.begin(), arr.end());
        for (auto& e : s)
        {
            auto target = ht.find(e);
            if (target)
                std::cout << target->kv_.first << target->kv_.second << std::endl;
        }
    }

    void compare_container()
    {
        const size_t N = 1000000;

        std::unordered_set<int> us;
        std::set<int> s;
        HashTable<int, int> ht;

        std::vector<int> v;
        v.reserve(N);
        //srand(time(0));
        for (size_t i = 0; i < N; ++i)
        {
            //v.push_back(rand()); // N比较大时，重复值比较多
            //v.push_back(rand() + i); // 重复值相对少
            //v.push_back(i); // 没有重复，有序
            v.push_back(rand() * 100 + rand() * 10 + rand());
        }

        size_t begin1 = clock();
        for (auto e : v)
        {
            s.insert(e);
        }
        size_t end1 = clock();
        std::cout << "set insert:" << end1 - begin1 << std::endl;

        size_t begin2 = clock();
        for (auto e : v)
        {
            us.insert(e);
        }
        size_t end2 = clock();
        std::cout << "unordered_set insert:" << end2 - begin2 << std::endl;

        size_t begin3 = clock();
        for (auto e : v)
        {
            ht.insert(std::make_pair(e, e));
        }
        size_t end3 = clock();
        std::cout << "HashTable insert:" << end3 - begin3 << std::endl << std::endl;

        std::cout << "set插入数据个数：" << s.size() << std::endl << std::endl;
        std::cout << "unordered_set插入数据个数：" << us.size() << std::endl << std::endl;
        ht.print_info();

        size_t begin4 = clock();
        for (auto e : v)
        {
            auto i = s.find(e);
        }
        size_t end4 = clock();
        std::cout << "set find:" << end4 - begin4 << std::endl;

        size_t begin5 = clock();
        for (auto e : v)
        {
            auto i = us.find(e);
        }
        size_t end5 = clock();
        std::cout << "unordered_set find:" << end5 - begin5 << std::endl;

        size_t begin6 = clock();
        for (auto e : v)
        {
            ht.find(e);
        }
        size_t end6 = clock();
        std::cout << "HashTable find:" << end6 - begin6 << std::endl << std::endl;

        size_t begin7 = clock();
        for (auto e : v)
        {
            s.erase(e);
        }
        size_t end7 = clock();
        std::cout << "set erase:" << end7 - begin7 << std::endl;

        size_t begin8 = clock();
        for (auto e : v)
        {
            us.erase(e);
        }
        size_t end8 = clock();
        std::cout << "unordered_set erase:" << end8 - begin8 << std::endl;

        size_t begin9 = clock();
        for (auto e : v)
        {
            ht.erase(e);
        }
        size_t end9 = clock();
        std::cout << "HashTable Erase:" << end9 - begin9 << std::endl << std::endl;
    }
}