#pragma once
#include <string>
#include "my_rbtree_v2.hpp"

namespace mystd
{
    template<class K, class V>
    class map
    {
        typedef K key_type;
        typedef std::pair<const K, V> value_type;

        class MapKeyOfT
        {
        public:
            const K& operator()(const std::pair<K, V>& kv)
            {
                return kv.first;
            }
        };
    public:
        typedef typename RBTree<K, value_type, MapKeyOfT>::iterator iterator;
        typedef typename RBTree<K, value_type, MapKeyOfT>::const_iterator const_iterator;

        iterator begin() const
        {
            return t_.begin();
        }

        iterator end() const
        {
            return t_.end();
        }

        std::pair<iterator, bool> insert(const std::pair<K, V>& kv)
        {
            return t_.insert(std::pair<const K, V>(kv));
        }

        V& operator[](const K& key)
        {
            std::pair<iterator, bool> ret = insert(std::make_pair(key, V()));
            return ret.first->second;
        }

    private:
        RBTree<key_type, value_type, MapKeyOfT> t_;
    };

    void test_map()
    {
        map<std::string, int> dict;

        dict.insert(std::make_pair("one", 1));
        dict.insert(std::make_pair("two", 2));
        dict.insert(std::make_pair("three", 3));
        dict.insert(std::make_pair("four", 4));

        map<std::string, int>::iterator it = dict.begin();
        while (it != dict.end())
        {
            //it->first += 'x';
            it->second += 10;

            std::cout << it->first << ":" << it->second << std::endl;
            ++it;
        }
        std::cout << std::endl;

        std::string words[] = { "apple", "banana", "orange", "grape", "melon" ,"apple", "banana", "orange" };
        map<std::string, int> count_map;
        for (auto& e : words)
        {
            count_map[e]++;
        }

        for (auto& kv : count_map)
        {
            std::cout << kv.first << ":" << kv.second << std::endl;
        }
        std::cout << std::endl;
    }
}