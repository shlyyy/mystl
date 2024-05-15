#pragma once
#include <bitset>
#include <string>
//#include <stdbool.h>
#include <iostream>
#include <vector>

struct BKDRHash
{
    size_t operator()(const std::string& key)
    {
        size_t hash = 0;
        for (auto e : key)
        {
            hash *= 31;
            hash += e;
        }

        return hash;
    }
};

struct APHash
{
    size_t operator()(const std::string& key)
    {
        size_t hash = 0;
        for (size_t i = 0; i < key.size(); i++)
        {
            char ch = key[i];
            if ((i & 1) == 0)
            {
                hash ^= ((hash << 7) ^ ch ^ (hash >> 3));
            }
            else
            {
                hash ^= (~((hash << 11) ^ ch ^ (hash >> 5)));
            }
        }
        return hash;
    }
};

struct DJBHash
{
    size_t operator()(const std::string& key)
    {
        size_t hash = 5381;
        for (auto ch : key)
        {
            hash += (hash << 5) + ch;
        }
        return hash;
    }
};

template<size_t N, class K = std::string,
    class HashFunc1 = BKDRHash, class HashFunc2 = APHash, class HashFunc3 = DJBHash>
class BloomFilter
{
public:
    void set(const K& key)
    {
        size_t hash1 = HashFunc1()(key) % N;
        size_t hash2 = HashFunc2()(key) % N;
        size_t hash3 = HashFunc3()(key) % N;

        bs_.set(hash1);
        bs_.set(hash2);
        bs_.set(hash3);
    }

    bool test(const K& key)
    {
        // 判断不存在是准确的
        size_t hash1 = HashFunc1()(key) % N;
        if (bs_.test(hash1) == false)
            return false;

        size_t hash2 = HashFunc2()(key) % N;
        if (bs_.test(hash2) == false)
            return false;

        size_t hash3 = HashFunc3()(key) % N;
        if (bs_.test(hash3) == false)
            return false;

        // 可能存在，也可能误判
        return true;
    }

private:
    std::bitset<N> bs_;
};

void test_bloom_filter()
{
    srand((unsigned int)time(0));
    const size_t N = 100000;
    BloomFilter<N * 10> bf;

    std::vector<std::string> v1;
    //std::string url = "https://www.cnblogs.com/-clq/archive/2012/05/31/2528153.html";
    std::string url = "猪八戒";

    for (size_t i = 0; i < N; ++i)
    {
        v1.push_back(url + std::to_string(i));
    }

    for (auto& str : v1)
    {
        bf.set(str);
    }

    // v2跟v1是相似字符串集（前缀一样），但是不一样
    std::vector<std::string> v2;
    for (size_t i = 0; i < N; ++i)
    {
        std::string urlstr = url;
        urlstr += std::to_string(9999999 + i);
        v2.push_back(urlstr);
    }

    size_t n2 = 0;
    for (auto& str : v2)
    {
        if (bf.test(str)) // 误判
        {
            ++n2;
        }
    }
    std::cout << "相似字符串误判率:" << (double)n2 / (double)N << std::endl;

    // 不相似字符串集
    std::vector<std::string> v3;
    for (size_t i = 0; i < N; ++i)
    {
        //string url = "zhihu.com";
        std::string url = "孙悟空";
        url += std::to_string(i + rand());
        v3.push_back(url);
    }

    size_t n3 = 0;
    for (auto& str : v3)
    {
        if (bf.test(str))
        {
            ++n3;
        }
    }
    std::cout << "不相似字符串误判率:" << (double)n3 / (double)N << std::endl;
}

void test_bloom_filter1()
{
    BloomFilter<100> bf;
    bf.set("猪八戒");
    bf.set("沙悟净");
    bf.set("孙悟空");
    bf.set("二郎神");

    std::cout << std::boolalpha << bf.test("猪八戒") << std::endl;
    std::cout << std::boolalpha << bf.test("沙悟净") << std::endl;
    std::cout << std::boolalpha << bf.test("孙悟空") << std::endl;
    std::cout << std::boolalpha << bf.test("二郎神") << std::endl;
    std::cout << std::boolalpha << bf.test("二郎神1") << std::endl;
    std::cout << std::boolalpha << bf.test("二郎神2") << std::endl;
    std::cout << std::boolalpha << bf.test("二郎神 ") << std::endl;
    std::cout << std::boolalpha << bf.test("太白晶星") << std::endl;
}