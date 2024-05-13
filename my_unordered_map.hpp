#pragma once
#include "my_hash_table.hpp"

namespace my_unordered_map
{
    template<class K, class V, class Hash = HashFuncs<K>>
    class unordered_map
    {
        typedef K key_type;
        typedef std::pair<const K, V> value_type;

        class MapKeyOfT
        {
        public:
            const K& operator()(const std::pair<K, V>& data)
            {
                return data.first;
            }
        };
    public:
        typedef typename HashTable<key_type, value_type, MapKeyOfT, Hash>::iterator iterator;
        typedef typename HashTable<key_type, value_type, MapKeyOfT, Hash>::const_iterator const_iterator;
    public:
        std::pair<iterator, bool> insert(const std::pair<K, V>& kv)
        {
            return hash_table_.insert(kv);
        }

        bool erase(const K& key)
        {
            return hash_table_.erase(key);
        }

        iterator begin()
        {
            return hash_table_.begin();
        }

        iterator end()
        {
            return hash_table_.end();
        }

        const_iterator begin() const
        {
            return hash_table_.begin();
        }

        const_iterator end() const
        {
            return hash_table_.end();
        }

        V& operator[](const K& key)
        {
            std::pair<iterator, bool> ret = hash_table_.insert(std::make_pair(key, V()));
            return ret.first->second;
        }

        const V& operator[](const K& key) const
        {
            std::pair<iterator, bool> ret = hash_table_.insert(std::make_pair(key, V()));
            return ret.first->second;
        }

        iterator find(const K& key)
        {
            return hash_table_.find(key);
        }

    private:
        HashTable<key_type, value_type, MapKeyOfT, Hash> hash_table_;
    };

    void test_unordered_map()
    {
        unordered_map<std::string, int> dict;
        dict.insert(std::make_pair("apple", 0));
        dict.insert(std::make_pair("orange", 5));
        dict.insert(std::make_pair("banana", 3));
        dict.insert(std::make_pair("watermelon", 10));

        for (auto& kv : dict)
        {
            //kv.first += "grape";
            kv.second += 100;
            std::cout << kv.first << ":" << kv.second << std::endl;
        }
        std::cout << std::endl;

        std::string words[] = { "apple", "banana", "orange", "grape", "melon" ,"apple", "banana", "orange" ,"banana" };
        unordered_map<std::string, int> count_map;
        for (auto& e : words)
        {
            count_map[e]++;
        }

        for (auto& kv : count_map)
        {
            //kv.first += 'x';
            std::cout << kv.first << ":" << kv.second << std::endl;
        }
        std::cout << std::endl;

        std::cout << count_map.find("apple")->second << std::endl;
    }
}