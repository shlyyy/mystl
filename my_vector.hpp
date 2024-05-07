#pragma once

namespace mystd
{
    template<class T>
    class vector
    {
    public:
        using value_type = T;
        using iterator = value_type*;
        using size_type = size_t;

    public:
        vector() { }
        ~vector()
        {
            delete[] _start;
            _size = _capacity = 0;
        }

        size_type size() const { return _size; }
        size_type capacity() const { return _capacity; }

        iterator begin() const { return _start; }
        iterator end() const { return _start + _size; }

        value_type& operator[](size_type pos)
        {
            assert(pos < size());
            return _start[pos];
        }
        const value_type& operator[](size_type pos) const
        {
            assert(pos < size());
            return _start[pos];
        }

        void reserve(size_type n)
        {
            if (n > _capacity)
            {
                // 两倍扩容
                size_type new_capacity = ((n > 2 * _capacity) || (_capacity == 0)) ? n : 2 * _capacity;
                iterator new_start = new value_type[new_capacity];
                if (_start)
                {
                    for (int i = 0; i < _size; ++i)
                    {
                        new_start[i] = _start[i];
                    }
                    delete[] _start;
                }

                _start = new_start;
                //_size
                _capacity = new_capacity;
            }
        }

        void insert(iterator pos, const value_type& val = T())
        {
            assert(pos >= _start);
            assert(pos <= end());

            size_type sz = pos - _start;

            reserve(_size + 1);

            // 如果扩容，需要更新pos迭代器位置
            pos = _start + sz;

            iterator _end = end() - 1;
            while (_end >= pos)
            {
                *(_end + 1) = *_end;
                --_end;
            }

            *pos = val;
            ++_size;
        }

        void push_back(const value_type& val)
        {
            insert(end(), val);
        }

        iterator erase(iterator pos)
        {
            assert(pos >= _start);
            assert(pos < end());

            iterator it = pos + 1;
            iterator _end = end();
            while (it < _end)
            {
                *(it - 1) = *it;
                ++it;
            }

            --_size;

            return pos; // 返回删除位置下一个元素所在位置，依然是pos位置，只不过内容不同
        }

        void pop_back()
        {
            erase(end() - 1);
        }
    public:
        template<class InputIterator>
        vector(InputIterator first, InputIterator last)
        {
            while (first != last)
            {
                push_back(*first);
                ++first;
            }
        }

        vector(size_type n, const value_type& val = value_type())
        {
            reserve(n);
            for (int i = 0; i < n; ++i)
            {
                push_back(val);
            }
        }

        // vector<int> v(10, 0); 避免和迭代器区间构造歧义
        vector(int n, const value_type& val = value_type())
        {
            reserve(n);
            for (int i = 0; i < n; ++i)
            {
                push_back(val);
            }
        }

    private:
        iterator _start = nullptr;
        size_type _size = 0;
        size_type _capacity = 0;
    };
    void test_vector()
    {
        vector<string> v;
        v.push_back("1");
        v.push_back("2");
        v.push_back("3");
        v.push_back("4");
        v.push_back("5");

        for (auto e : v)
        {
            std::cout << e << std::endl;
        }
        std::cout << std::endl;

        v.erase(v.begin() + 1);
        for (auto e : v)
        {
            std::cout << e << std::endl;
        }
        std::cout << std::endl;

        v.pop_back();
        for (auto e : v)
        {
            std::cout << e << std::endl;
        }
        std::cout << std::endl;
    }
    void test_vector1()
    {
        vector<int> v;
        v.push_back(1);
        v.push_back(2);
        v.push_back(3);
        v.push_back(4);
        v.push_back(5);

        for (auto e : v)
        {
            std::cout << e << std::endl;
        }
        std::cout << std::endl;

        v.erase(v.begin() + 1);
        for (auto e : v)
        {
            std::cout << e << std::endl;
        }
        std::cout << std::endl;

        v.pop_back();
        for (auto e : v)
        {
            std::cout << e << std::endl;
        }
        std::cout << std::endl;
    }
}

namespace mystd1
{
    template <class T>
    inline void destroy(T* pointer) {
        pointer->~T();
    }

    template <class T1, class T2>
    inline void construct(T1* p, const T2& value) {
        new (p) T1(value); // 在空间p处，调用T1拷贝构造函数，并用value进行初始化
    }

    template<class T>
    class vector
    {
    public:
        typedef T value_type;
        typedef value_type* iterator;
        typedef const value_type* const_iterator;

        typedef size_t size_type;

    public:

    public:
        vector() : start(nullptr), finish(nullptr), end_of_storage(nullptr) {}

        ~vector()
        {
            destroy(start, finish);
            deallocate();
        }
    public:
        iterator allocate(size_t n)
        {
            return 0 == n ? 0 : (iterator)malloc(n * sizeof(value_type));
        }
        void deallocate()
        {
            if (start)
                free(start);
        }
        void destroy(iterator first, iterator last)
        {
            for (; first < last; ++first)
                mystd1::destroy(&*first);
        }
    public:
        iterator __copy_forward(iterator first, iterator last, iterator result)
        {
            iterator cur = result;
            for (; first != last; ++first, ++cur)
                construct(&*cur, *first);
            return cur;
        }

        iterator __copy_backward(iterator first, iterator last, iterator result)
        {
            while (first != last) *--result = *--last;
            return result;
        }

    public:

        iterator begin() { return start; }
        iterator end() { return finish; }
        const_iterator begin() const { return start; }
        const_iterator end() const { return finish; }

        size_type size() const { return size_type(end() - begin()); }
        size_type capacity() const { return size_type(end_of_storage - begin()); }

        value_type& operator[](size_t pos)
        {
            assert(pos < size());

            return start[pos];
        }

        void push_back(const T& x)
        {
            if (finish != end_of_storage) {
                construct(finish, x);
                ++finish;
            }
            else
                insert(finish, x);
        }

        void insert(iterator pos, const T& x)
        {
            assert(pos >= start);
            assert(pos <= finish);

            if (finish != end_of_storage)
            {
                // 无需扩容
                construct(finish, *(finish - 1));
                ++finish;
                //value_type x_copy = x; // ???
                __copy_backward(pos, finish - 2, finish - 1);
                *pos = x;
            }
            else
            {
                // 两倍扩容
                const size_type old_size = size();
                const size_type len = old_size != 0 ? 2 * old_size : 1;

                // 避免new T时调用T的构造函数
                //iterator new_start = new T[len];

                iterator new_start = allocate(len);
                iterator new_finish = new_start;

                // 拷贝 [start, pos)
                new_finish = __copy_forward(start, pos, new_start);

                // 拷贝 x 到 pos
                construct(new_finish, x);
                //*new_finish = x;
                ++new_finish;

                // 拷贝 (pos, finish)
                new_finish = __copy_forward(pos, finish, new_finish);

                // 释放原来空间并更新
                destroy(begin(), end());
                deallocate();
                start = new_start;
                finish = new_finish;
                end_of_storage = new_start + len;
            }
        }

    public:
        iterator start; // 第一个元素开始位置
        iterator finish; // 使用的最后一个元素下一个位置
        iterator end_of_storage; // 空间中最后一个元素下一个位置
    };

    class Student
    {
    public:
        Student(size_t age = 0, const char* name = "") : _age(age)
        {
            _name = new char[strlen(name) + 1];
            strcpy(_name, name);
            std::cout << this << " Constructor: name=" << _name << ", age=" << _age << std::endl;
        }

        Student(const Student& other) : _age(other._age)
        {
            _name = new char[strlen(other._name) + 1];
            strcpy(_name, other._name);
            std::cout << this << " Copy Constructor: name=" << _name << ", age=" << _age << std::endl;
        }

        Student& operator=(const Student& other)
        {
            if (&other != this) {
                delete[] _name;

                _age = other._age;
                _name = new char[strlen(other._name) + 1];
                strcpy(_name, other._name);
                std::cout << this << " Assign Operator: name=" << _name << ", age=" << _age << std::endl;
            }
            return *this;
        }

        ~Student()
        {
            std::cout << this << " Destructor: name=" << _name << ", age=" << _age << std::endl;
            delete[] _name;
        }

        const char* get_name() const { return _name; }
        size_t get_age() const { return _age; }

    private:
        size_t _age;
        char* _name;
    };

    std::ostream& operator <<(std::ostream& out, const Student& s)
    {
        out << "name=" << s.get_name() << ", age=" << s.get_age();
        return out;
    }

    void test_vector()
    {
        vector<Student> v;
        v.push_back(Student(10, "Jack1"));
        v.push_back(Student(20, "Jack2"));
        v.push_back(Student(30, "Jack3"));
        v.push_back(Student(40, "Jack4"));
        vector<Student>::iterator it = v.begin();
        while (it != v.end())
        {
            std::cout << it << " " << *it << std::endl;
            ++it;
        }
        v.push_back(Student(50, "Jack5"));
        /*v.push_back(Student(60, "Jack6"));*/
        v.insert(v.end() - 1, Student(60, "Jack6"));

        v.push_back(Student(70, "Jack7"));
        v.push_back(Student(80, "Jack8"));
        it = v.begin();
        while (it != v.end())
        {
            std::cout << it << " " << *it << std::endl;
            ++it;
        }

        std::cout << std::endl << std::endl;

        std::cout << "travel: " << std::endl;
        it = v.begin();
        while (it != v.end())
        {
            std::cout << it << " " << *it << std::endl;
            ++it;
        }

        /*std::cout << "for travel: " << std::endl;
        for (auto& e : v)
        {
            std::cout << e << std::endl;
        }*/
    }
}