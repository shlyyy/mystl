#pragma once
#include "my_rbtree_v2.hpp"

namespace mystd
{
    template<class K>
    class set
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
        typedef typename RBTree<K, K, SetKeyofT>::const_iterator iterator;
        typedef typename RBTree<K, K, SetKeyofT>::const_iterator const_iterator;

        iterator begin() const
        {
            return t_.begin();
        }

        iterator end() const
        {
            return t_.end();
        }

    public:
        std::pair<iterator, bool> insert(const K& key)
        {
            //std::pair<typename RBTree<K, K, SetKeyofT>::iterator, bool> p = t_.insert(key);
            //return std::pair<iterator, bool>(p.first, p.second);
            return t_.insert(key);
        }

    private:
        RBTree<key_type, value_type, SetKeyofT> t_;
    };

    void test_set()
    {
        set<int> s;
        s.insert(4);
        s.insert(1);
        s.insert(2);
        s.insert(3);
        s.insert(2);
        s.insert(0);
        s.insert(10);
        s.insert(5);

        set<int>::iterator it = s.begin();
        while (it != s.end())
        {
            //*it += 1;

            std::cout << *it << " ";
            ++it;
        }
        std::cout << std::endl;

        it = s.begin();
        // *it = 100;

        for (auto e : s)
        {
            std::cout << e << " ";
        }
        std::cout << std::endl;
    }
}