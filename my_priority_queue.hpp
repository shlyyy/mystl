#pragma once
#include <iostream>
#include <queue>

namespace mystd
{
    template<class T, class Container = std::vector<T>,
        class Compare = std::less<typename Container::value_type>
        /*class Compare = std::less<T>*/>
    class priority_queue
    {
    public:
        priority_queue() {}

        template <class InputIterator>
        priority_queue(InputIterator first, InputIterator last)
            :_con(first, last)
        {
            for (int i = (_con.size() - 2) / 2; i >= 0; --i)
            {
                adjust_down(i);
            }
        }

        void adjust_up(size_t child)
        {
            Compare comp;
            int parent = (int)(child - 1) / 2;
            while (child > 0)
            {
                if (comp(_con[parent], _con[child]))
                {
                    std::swap(_con[child], _con[parent]);
                    child = parent;
                    parent = (int)(child - 1) / 2;
                }
                else
                {
                    break;
                }
            }
        }

        void push(const T& val)
        {
            _con.push_back(val);
            adjust_up(_con.size() - 1);
        }

        void adjust_down(size_t parent)
        {
            Compare comp;
            size_t child = parent * 2 + 1;
            while (child < _con.size())
            {
                if (child + 1 < _con.size()
                    && comp(_con[child], _con[child + 1]))
                {
                    ++child;
                }

                if (comp(_con[parent], _con[child]))
                {
                    std::swap(_con[child], _con[parent]);
                    parent = child;
                    child = parent * 2 + 1;
                }
                else
                {
                    break;
                }
            }
        }

        void pop()
        {
            std::swap(_con[0], _con[_con.size() - 1]);
            _con.pop_back();
            adjust_down(0);
        }

        const T& top()
        {
            return _con[0];
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

    /*template<class T, class Container = std::vector<T>>
    class priority_queue
    {
    public:
        priority_queue() {}

        template <class InputIterator>
        priority_queue(InputIterator first, InputIterator last)
            :_con(first, last)
        {
            for (int i = (_con.size() - 2) / 2; i >= 0; --i)
            {
                adjust_down(i);
            }
        }

        void adjust_up(size_t child)
        {
            int parent = (int)(child - 1) / 2;
            while (child > 0)
            {
                if (_con[parent] < _con[child])
                {
                    std::swap(_con[child], _con[parent]);
                    child = parent;
                    parent = (int)(child - 1) / 2;
                }
                else
                {
                    break;
                }
            }
        }

        void push(const T& val)
        {
            _con.push_back(val);
            adjust_up(_con.size() - 1);
        }

        void adjust_down(size_t parent)
        {
            size_t child = parent * 2 + 1;
            while (child < _con.size())
            {
                if (child + 1 < _con.size()
                    && _con[child] < _con[child + 1])
                {
                    ++child;
                }

                if (_con[parent] < _con[child])
                {
                    std::swap(_con[child], _con[parent]);
                    parent = child;
                    child = parent * 2 + 1;
                }
                else
                {
                    break;
                }
            }
        }

        void pop()
        {
            std::swap(_con[0], _con[_con.size() - 1]);
            _con.pop_back();
            adjust_down(0);
        }

        const T& top()
        {
            return _con[0];
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
    };*/

    // ·Âº¯Êý
    template<class T>
    class Less
    {
    public:
        bool operator()(const T& x, const T& y)
        {
            return x < y;
        }
    };

    void test_priority_queue2()
    {
        Less<int> less;
        std::cout << less(2, 3) << std::endl;
    }

    void test_priority_queue()
    {
        priority_queue<int> q;
        q.push(3);
        q.push(1);
        q.push(5);
        q.push(4);

        while (!q.empty())
        {
            std::cout << q.top() << std::endl;
            q.pop();
        }
    }
}