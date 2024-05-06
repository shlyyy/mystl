#include "my_string.hpp"
#include "my_vector.hpp"
#include <iostream>
#include <vector>
std::vector<int> v;
int main(int argc, char* argv[])
{
    std::cout << "begin main" << std::endl;

    mystd::test_vector();

    std::cout << "end main" << std::endl;
    return 0;
}