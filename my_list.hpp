#pragma once
#include <list>
#include <algorithm>
#include <iostream>
#include <string>

namespace mystd
{
    template<class T>
    class list_node
    {
    public:
        list_node(const T& val = T()) : _prev(nullptr), _next(nullptr), _data(val) {}
    public:
        T _data;
        list_node* _prev;
        list_node* _next;
    };

    /*template<class T>
    class list_iterator
    {
    public:
        typedef list_node<T> Node;
        typedef list_iterator<T> self;
    public:
        list_iterator(Node* node) : _pnode(node) {}

        self& operator++()
        {
            _pnode = _pnode->_next;
            return *this;
        }

        T& operator*()
        {
            return _pnode->_data;
        }

        T* operator->()
        {
            return &_pnode->_data;
        }

        bool operator!=(const self& s)
        {
            return _pnode != s._pnode;
        }

        self& operator--()
        {
            _pnode = _pnode->_prev;
            return *this;
        }

    public:
        Node* _pnode;
    };

    template<class T>
    class const_list_iterator
    {
    public:
        typedef list_node<T> Node;
        typedef const_list_iterator<T> self;
    public:
        const_list_iterator(Node* node) : _pnode(node) {}

        self& operator++()
        {
            _pnode = _pnode->_next;
            return *this;
        }

        const T& operator*()
        {
            return _pnode->_data;
        }

        const T* operator->()
        {
            return &_pnode->_data;
        }

        bool operator!=(const self& s)
        {
            return _pnode != s._pnode;
        }

        self& operator--()
        {
            _pnode = _pnode->_prev;
            return *this;
        }

    public:
        Node* _pnode;
    };*/

    template<class T, class Ref, class Ptr>
    class list_iterator
    {
    public:
        typedef list_node<T> Node;
        typedef list_iterator<T, Ref, Ptr> self;
    public:
        list_iterator(Node* node) : _pnode(node) {}

        self& operator++()
        {
            _pnode = _pnode->_next;
            return *this;
        }

        Ref operator*()
        {
            return _pnode->_data;
        }

        Ptr operator->()
        {
            return &_pnode->_data;
        }

        bool operator!=(const self& s)
        {
            return _pnode != s._pnode;
        }

        self& operator--()
        {
            _pnode = _pnode->_prev;
            return *this;
        }

        self operator++(int)
        {
            self tmp(*this);
            _pnode = _pnode->_next;

            return tmp;
        }

        self operator--(int)
        {
            self tmp(*this);
            _pnode = _pnode->_prev;

            return tmp;
        }

        bool operator==(const self& s)
        {
            return _pnode == s._pnode;
        }

    public:
        Node* _pnode;
    };

    template<class T>
    class list
    {
        typedef list_node<T> Node;
        typedef list<T> self;
    public:
        typedef list_iterator<T, T&, T*> iterator;
        typedef list_iterator<T, const T&, const T*> const_iterator;
    public:
        list() { empty_init(); }

        void empty_init()
        {
            _head = new Node;
            _head->_next = _head;
            _head->_prev = _head;
        }

        size_t size() const { return _size; }

        iterator begin() { return _head->_next; }
        iterator end() { return _head; }

        iterator insert(iterator pos, const T& val)
        {
            Node* cur = pos._pnode;
            Node* new_node = new Node(val);
            Node* prev = cur->_prev;

            prev->_next = new_node;
            new_node->_prev = prev;

            new_node->_next = cur;
            cur->_prev = new_node;

            ++_size;
            return new_node;
        }

        void push_back(const T& val)
        {
            insert(end(), val);
        }

        void push_front(const T& val)
        {
            insert(begin(), val);
        }

        iterator erase(iterator pos)
        {
            Node* cur = pos._pnode;
            Node* prev = cur->_prev;
            Node* next = cur->_next;

            delete cur;

            prev->_next = next;
            next->_prev = prev;

            --_size;

            return next;
        }

        void pop_back()
        {
            erase(--end());
        }

        void pop_front()
        {
            erase(begin());
        }

        list(const self& lt)
        {
            empty_init();
            for (auto e : lt)
            {
                push_back(e);
            }
        }

        /*self& operator=(const self& lt)
        {
            if (this != &lt)
            {
                clear();
                for (auto e : lt)
                {
                    push_back(e);
                }
            }

            return *this;
        }*/

        void clear()
        {
            iterator it = begin();
            while (it != end())
            {
                it = erase(it);
            }
        }

        self& operator=(self lt)
        {
            swap(lt);

            return *this;
        }

        void swap(self& lt)
        {
            std::swap(_head, lt._head);
            std::swap(_size, lt._size);
        }

        ~list()
        {
            clear();

            delete _head;
            _head = nullptr;
        }

    public:
        Node* _head;
        size_t _size;
    };

    void test_list()
    {
        list<string> lt;
        lt.push_back("1");
        lt.push_back("2");
        lt.push_back("3");
        lt.push_back("4");
        lt.push_back("5");

        list<string>::iterator it = lt.begin();
        while (it != lt.end())
        {
            std::cout << it->c_str() << std::endl;
            //std::cout << it.operator->()->c_str() << std::endl;
            ++it;
        }
        std::cout << std::endl;
    }
    /*template<class Container>
    void print_container(const Container& con)
    {
        typename Container::const_iterator it = con.begin();
        while (it != con.end())
        {
            std::cout << *it << " ";
            ++it;
        }
        std::cout << std::endl;
    }

    void test_list3()
    {
        std::cout << "hello" << std::endl;

        std::list<int> lt;
        lt.push_back(1);
        lt.push_back(2);
        lt.push_back(3);
        lt.push_back(4);
        lt.push_back(5);
        print_container(lt);

        std::list<string> lt1;
        lt1.push_back("1111111111111");
        lt1.push_back("1111111111111");
        lt1.push_back("1111111111111");
        lt1.push_back("1111111111111");
        lt1.push_back("1111111111111");
        print_container(lt1);
    }*/

    /*template<class T>
    void print_list(const std::list<T>& lt)
    {
        typename std::list<T>::const_iterator it = lt.begin();
        while (it != lt.end())
        {
            std::cout << *it << " ";
            ++it;
        }
        std::cout << std::endl;
    }*/

    /*void test_list2()
    {
        std::cout << "hello" << std::endl;

        std::list<int> lt;
        lt.push_back(1);
        lt.push_back(2);
        lt.push_back(3);
        lt.push_back(4);
        lt.push_back(5);
        print_list(lt);

        std::list<string> lt1;
        lt1.push_back("1111111111111");
        lt1.push_back("1111111111111");
        lt1.push_back("1111111111111");
        lt1.push_back("1111111111111");
        lt1.push_back("1111111111111");
        print_list(lt1);
    }*/

    void test_list1()
    {
        srand((unsigned int)time(0));
        const int N = 1000000;

        std::list<int> lt1;
        std::list<int> lt2;

        for (int i = 0; i < N; ++i)
        {
            auto e = rand();
            lt1.push_back(e);
            lt2.push_back(e);
        }

        int begin1 = clock();
        vector<int> v(lt2.begin(), lt2.end());
        std::sort(v.begin(), v.end());
        //lt2.assign(v.begin(), v.end());
        int end1 = clock();

        int begin2 = clock();
        lt1.sort();
        int end2 = clock();

        printf("list copy vector sort copy list sort:%d\n", end1 - begin1);
        printf("list sort:%d\n", end2 - begin2);
    }
}