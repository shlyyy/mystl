#pragma once
#include "my_hash_table.hpp"

namespace my_unordered_set
{
    template<class K, class Hash = HashFuncs<K>>
    class unordered_set
    {
        typedef K key_type;
        typedef K value_type;

        class SetKeyofT
        {
        public:
            const K& operator()(const K& key)
            {
                return key;
            }
        };
    public:
        typedef typename HashTable<key_type, value_type, SetKeyofT, Hash>::const_iterator iterator;
        typedef typename HashTable<key_type, value_type, SetKeyofT, Hash>::const_iterator const_iterator;
    public:
        std::pair<iterator, bool> insert(const K& key)
        {
            return hash_table_.insert(key);
        }

        bool erase(const K& key)
        {
            return hash_table_.erase(key);
        }

        const_iterator begin() const
        {
            return hash_table_.begin();
        }

        const_iterator end() const
        {
            return hash_table_.end();
        }

    private:
        HashTable<key_type, value_type, SetKeyofT, Hash> hash_table_;
    };

    void test_unordered_set()
    {
        unordered_set<int> us;
        us.insert(5);
        us.insert(15);
        us.insert(52);
        us.insert(3);

        unordered_set<int>::iterator it = us.begin();
        while (it != us.end())
        {
            //*it += 5;
            std::cout << *it << " ";
            ++it;
        }
        std::cout << std::endl;

        for (auto e : us)
        {
            std::cout << e << " ";
        }
        std::cout << std::endl;
    }
}