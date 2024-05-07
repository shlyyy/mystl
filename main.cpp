#include "my_string.hpp"
#include "my_vector.hpp"
#include "my_list.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    std::cout << "begin main" << std::endl;

    mystd::test_list();

    std::cout << "end main" << std::endl;
    return 0;
}