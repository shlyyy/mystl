#pragma once
#include <vector>
#include <iostream>

namespace my_bitset
{
    template<size_t N>
    class bitset
    {
    public:
        bitset()
        {
            bitset_.resize((N >> 5) + 1, 0); // N个bit需要N/32个int型
        }

        void set(size_t pos)
        {
            size_t i = pos / 32;
            size_t j = pos % 32;
            bitset_[i] |= (1 << j);
        }

        void reset(size_t pos)
        {
            size_t i = pos / 32;
            size_t j = pos % 32;
            bitset_[i] &= ~(1 << j);
        }

        bool test(size_t pos)
        {
            size_t i = pos / 32;
            size_t j = pos % 32;
            return bitset_[i] & (1 << j);
        }

    private:
        std::vector<int> bitset_;
    };

    //template<size_t N>
    //class twobitset
    //{
    //public:
    //    void set(size_t x)
    //    {
    //        //00->01
    //        //01->10
    //        //10 不变  出现两次及以上
    //        if (_bs1.test(x) == false && _bs2.test(x) == false)
    //        {
    //            // 出现一次，变为01
    //            _bs2.set(x);
    //        }
    //        else if (_bs1.test(x) == false && _bs2.test(x) == true)
    //        {
    //            // 第二次及以上出现，变为10
    //            _bs1.set(x);
    //            _bs2.reset(x);
    //        }
    //    }

    //    void PrintOnce()
    //    {
    //        for (size_t i = 0; i < N; i++)
    //        {
    //            if (_bs1.test(i) == false && _bs2.test(i) == true)
    //            {
    //                std::cout << i << std::endl;
    //            }
    //        }
    //        std::cout << std::endl;
    //    }

    //private:
    //    bitset<N> _bs1;
    //    bitset<N> _bs2;
    //};

    //void test_bitset()
    //{
    //    int a[] = { 1, 4, 7, 9, 44, 88, 1, 4, 88, 99, 78, 5, 7, 7, 7, 7 };
    //    twobitset<100> bs;
    //    for (auto e : a)
    //    {
    //        bs.set(e);
    //    }
    //    bs.PrintOnce();
    //}

    template<size_t N>
    class twobitset
    {
    public:
        void set(size_t x)
        {
            //00->01
            //01->10
            //10->11
            //11->不变
            if (_bs1.test(x) == false && _bs2.test(x) == false)
            {
                _bs2.set(x);
            }
            else if (_bs1.test(x) == false && _bs2.test(x) == true)
            {
                _bs1.set(x);
                _bs2.reset(x);
            }
            else if (_bs1.test(x) == true && _bs2.test(x) == false)
            {
                _bs1.set(x);
                _bs2.set(x);
            }
        }

        void Print()
        {
            for (size_t i = 0; i < N; i++)
            {
                if (_bs1.test(i) == false && _bs2.test(i) == true)
                {
                    std::cout << "1->" << i << std::endl;
                }
                else if (_bs1.test(i) == true && _bs2.test(i) == false)
                {
                    std::cout << "2->" << i << std::endl;
                }
            }
            std::cout << std::endl;
        }

    private:
        bitset<N> _bs1;
        bitset<N> _bs2;
    };

    void test_bitset()
    {
        int a[] = { 1, 4, 7, 9, 44, 88, 1, 4, 88, 99, 78, 5, 7, 7, 7, 7 };
        twobitset<100> bs;
        for (auto e : a)
        {
            bs.set(e);
        }
        bs.Print();
    }
}