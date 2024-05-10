#include "my_string.hpp"
#include "my_vector.hpp"
#include "my_list.hpp"
#include "my_stack.hpp"
#include "my_queue.hpp"
#include "my_priority_queue.hpp"
#include "my_binary_search_tree.hpp"

#include <iostream>

int main(int argc, char* argv[])
{
    std::cout << "begin main" << std::endl;

    //mystd::test_priority_queue();
    mypair::test_binary_search_tree_kv();

    std::cout << "end main" << std::endl;
    return 0;
}