#pragma once
#include <iostream>
#include <stack>
#include <deque>

namespace mystd
{
    template<class T, class Container = std::deque<T>>
    class stack
    {
    public:
        void push(const T& x)
        {
            _con.push_back(x);
        }

        void pop()
        {
            _con.pop_back();
        }

        const T& top()
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

    void test_stack()
    {
        stack<int> s;
        s.push(1);
        s.push(2);
        s.push(3);
        s.push(4);
        while (!s.empty())
        {
            std::cout << s.top() << std::endl;
            s.pop();
        }
    }
}