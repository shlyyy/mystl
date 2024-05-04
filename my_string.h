#pragma once
#include <string.h>
#include <assert.h>
#include <iostream>

namespace my_std
{
    class string
    {
    public:
        typedef char* iterator;

        iterator begin()
        {
            return _str;
        }

        iterator end()
        {
            return _str + _size;
        }
    public:
        string(const char* str = "")
            : _size(strlen(str))
        {
            _capacity = (_size <= 15) ? 15 : _size;

            _str = new char[_capacity + 1];
            strcpy(_str, str);
        }

        string(const string& s)
        {
            // apply the new space
            _size = s._size;
            _capacity = s._size;
            _str = new char[_capacity + 1];

            // copy str
            strcpy(_str, s.c_str());
        }

        string& operator=(const string& s)
        {
            if (&s != this)
            {
                if (s._size > _capacity)
                {
                    // 两倍扩容不够，则扩容到s._size
                    _capacity = (s._size > 2 * _capacity) ? s._size : 2 * _capacity;

                    delete[] _str;
                    _str = new char[_capacity + 1];
                }
                // reserve(s._size);

                strcpy(_str, s._str);
                _size = s._size;
            }
            return *this;
        }

        ~string()
        {
            delete[] _str;
            _str = nullptr;
            _size = _capacity = 0;
        }

        const char* c_str() const
        {
            return _str;
        }

        size_t size() const
        {
            return _size;
        }

        size_t capacity() const
        {
            return _capacity;
        }

        char& operator[](size_t pos)
        {
            assert(pos < _size);

            return _str[pos];
        }

        const char& operator[](size_t pos) const
        {
            assert(pos < _size);

            return _str[pos];
        }

        // expand the capacity
        void reserve(size_t n)
        {
            if (n > _capacity)
            {
                // 两倍扩容不够，则扩容到n
                _capacity = (n > 2 * _capacity) ? n : 2 * _capacity;

                char* tmp = new char[_capacity + 1];
                strcpy(tmp, _str);

                delete[] _str;
                _str = tmp;
            }
        }

        void push_back(char ch)
        {
            /*if (_size == _capacity)
            {
                reserve(_capacity * 2);
            }*/

            reserve(_size + 1);

            _str[_size++] = ch;
            _str[_size] = '\0';
        }

        void append(const char* str)
        {
            size_t len = strlen(str);
            reserve(_size + len);

            strcpy(_str + _size, str);
            _size += len;
        }

        string& operator+=(const char* str)
        {
            append(str);
            return *this;
        }

        string& operator+=(char ch)
        {
            push_back(ch);
            return *this;
        }

        void insert(size_t pos, char ch)
        {
            assert(pos <= _size);
            reserve(_size + 1);

            /*
            size_t end = _size; // from '\0' start
            while (end >= pos)  // when pos=0, while loop will not exit
            {
                _str[end + 1] = _str[end];
                --end;
            }
            */

            size_t end = _size + 1; // from '\0' next pos start
            while (end > pos)
            {
                _str[end] = _str[end - 1];
                --end;
            }

            _str[pos] = ch;
            _size++;
        }

        void insert(size_t pos, const char* str)
        {
            assert(pos <= _size);

            size_t len = strlen(str);

            reserve(_size + len);

            // 向后挪动数据
            size_t end = _size + 1; // from '\0' next pos start
            while (end > pos)
            {
                _str[end - 1 + len] = _str[end - 1];
                --end;
            }

            // 拷贝数据
            /*for (int i = 0; i < len; ++i)
            {
                _str[i + pos] = str[i];
            }*/

            strncpy(_str + pos, str, len);

            _size += len;
        }

        void erase(size_t pos, size_t len = npos)
        {
            assert(pos < _size);
            if (len == npos || pos + len >= _size)
            {
                // 从pos删除到结尾
                _str[pos] = '\0';
                _size = pos;
            }
            else
            {
                // 从pos删除len长度个
                strcpy(_str + pos, _str + pos + len);
                _size -= len;
            }
        }

        bool operator<(const string& s) const
        {
            return strcmp(_str, s._str) < 0;
        }

        bool operator==(const string& s) const
        {
            return strcmp(_str, s._str) == 0;
        }

        bool operator<=(const string& s) const
        {
            return *this < s || *this == s;
        }

        bool operator>(const string& s) const
        {
            return !(*this <= s);
        }

        bool operator>=(const string& s) const
        {
            return !(*this < s);
        }

        bool operator!=(const string& s) const
        {
            return !(*this == s);
        }

        void clear()
        {
            _str[0] = '\0';
            _size = 0;
        }

        void resize(size_t n, char ch = '\0')
        {
            if (n <= _size)
            {
                _str[n] = '\0';
                _size = n;
            }
            else
            {
                reserve(n);
                while (_size < n)
                {
                    _str[_size] = ch;
                    ++_size;
                }

                _str[_size] = '\0';
            }
        }

        size_t find(char ch, size_t pos = 0)
        {
            for (size_t i = pos; i < _size; i++)
            {
                if (_str[i] == ch)
                {
                    return i;
                }
            }
            return npos;
        }

        size_t find(const char* sub, size_t pos = 0)
        {
            const char* p = strstr(_str + pos, sub);
            if (p)
            {
                return p - _str;
            }
            else
            {
                return npos;
            }
        }

        string substr(size_t pos, size_t len = npos)
        {
            string s;
            size_t end = pos + len;
            if (len == npos || end >= _size) // 取到结尾
            {
                // len为字串的size
                len = _size - pos; // end >=_size
                end = _size;
            }

            s.reserve(len);
            for (size_t i = pos; i < end; i++)
            {
                s += _str[i];
            }

            return s;
        }

    private:
        char* _str;
        size_t _size;
        size_t _capacity;

        const static size_t npos;
    };

    const size_t string::npos = -1;

    std::ostream& operator <<(std::ostream& out, const string& s)
    {
        out << s.c_str();
        return out;
    }

    std::istream& operator>>(std::istream& in, string& s)
    {
        s.clear();

        char buf[128] = { '\0' };
        size_t i = 0;

        char ch;
        // in >> ch;
        ch = in.get(); // istream::get()
        while (ch != ' ' && ch != '\n')
        {
            //s += ch;
            if (i == 127)
            {
                s += buf;
                i = 0;
            }
            buf[i] = ch;
            i++;

            ch = in.get();
        }

        if (i >= 0)
        {
            buf[i] = '\0';
            s += buf;
        }

        return in;
    }
}
