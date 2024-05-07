#include "my_string.hpp"
#include "my_vector.hpp"
#include "my_list.hpp"
#include "my_stack.hpp"
#include "my_queue.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    std::cout << "begin main" << std::endl;

    mystd::test_queue();

    std::cout << "end main" << std::endl;
    return 0;
}