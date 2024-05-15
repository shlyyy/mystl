//#include "my_string.hpp"
//#include "my_vector.hpp"
//#include "my_list.hpp"
//#include "my_stack.hpp"
//#include "my_queue.hpp"
//#include "my_priority_queue.hpp"
//#include "my_binary_search_tree.hpp"
//#include "my_avl.hpp"
//#include "my_rbtree_v2.hpp"
//#include "my_set.hpp"
//#include "my_map.hpp"
//#include "my_hash_table.hpp"
//#include "my_unordered_set.hpp"
//#include "my_unordered_map.hpp"

//#include "my_bitset.hpp"
#include "my_bloom_filter.hpp"

#include <iostream>

int main(int argc, char* argv[])
{
    std::cout << "begin main" << std::endl;

    test_bloom_filter();

    std::cout << "end main" << std::endl;
    return 0;
}