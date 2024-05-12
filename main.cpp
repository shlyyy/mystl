#include "my_string.hpp"
#include "my_vector.hpp"
#include "my_list.hpp"
#include "my_stack.hpp"
#include "my_queue.hpp"
#include "my_priority_queue.hpp"
#include "my_binary_search_tree.hpp"
#include "my_avl.hpp"
//#include "my_rbtree_v2.hpp"
#include "my_set.hpp"
#include "my_map.hpp"

#include <iostream>

int main(int argc, char* argv[])
{
    std::cout << "begin main" << std::endl;

    mystd::test_set();
    mystd::test_map();

    std::cout << "end main" << std::endl;
    return 0;
}