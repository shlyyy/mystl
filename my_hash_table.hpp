#pragma once
#include <vector>
#include <string>
#include <iostream>

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
}