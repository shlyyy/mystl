#pragma once

namespace mystd
{
    template <class T>
    inline void destroy(T* pointer) {
        pointer->~T();
    }

    template <class T1, class T2>
    inline void construct(T1* p, const T2& value) {
        new (p) T1(value); // �ڿռ�p��������T1�������캯��������value���г�ʼ��
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
                mystd::destroy(&*first);
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
                // ��������
                construct(finish, *(finish - 1));
                ++finish;
                //value_type x_copy = x; // ???
                __copy_backward(pos, finish - 2, finish - 1);
                *pos = x;
            }
            else
            {
                // ��������
                const size_type old_size = size();
                const size_type len = old_size != 0 ? 2 * old_size : 1;

                // ����new Tʱ����T�Ĺ��캯��
                //iterator new_start = new T[len];

                iterator new_start = allocate(len);
                iterator new_finish = new_start;

                // ���� [start, pos)
                new_finish = __copy_forward(start, pos, new_start);

                // ���� x �� pos
                construct(new_finish, x);
                //*new_finish = x;
                ++new_finish;

                // ���� (pos, finish)
                new_finish = __copy_forward(pos, finish, new_finish);

                // �ͷ�ԭ���ռ䲢����
                destroy(begin(), end());
                deallocate();
                start = new_start;
                finish = new_finish;
                end_of_storage = new_start + len;
            }
        }

    public:
        iterator start; // ��һ��Ԫ�ؿ�ʼλ��
        iterator finish; // ʹ�õ����һ��Ԫ����һ��λ��
        iterator end_of_storage; // �ռ������һ��Ԫ����һ��λ��
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