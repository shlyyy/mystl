#pragma once
#include <iostream>
#include <deque>
#include <queue>

namespace mystd
{
    template<class T, class Container = std::deque<T>>
    class queue
    {
    public:
        void push(const T& x)
        {
            _con.push_back(x);
        }

        void pop()
        {
            _con.pop_front();
        }

        const T& front()
        {
            return _con.front();
        }

        const T& back()
        {
            return _con.back();
        }

        bool empty()
        {
            return _con.empty();
        }

        size_t size()
        {
            return _con.size();
        }

    private:
        Container _con;
    };

    void test_queue()
    {
        queue<int> q;
        q.push(1);
        q.push(2);
        q.push(3);

        while (!q.empty())
        {
            int frontElement = q.front();
            std::cout << frontElement << " " << std::endl;
            q.pop();
        }
    }
}