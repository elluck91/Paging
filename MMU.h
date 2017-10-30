#include <list>
#include <string>
#include <iostream>
using namespace std;
#include "Memory.h"
class MMU {
    private:
        Memory page_table[100];
        list<Memory> free_memory;
        double time_stamp;
        string process_to_evict;
        int algo;
        string virtual_address_str;
        int virtual_address_num;
        int empty_page_index;
        Memory temp_mem;
        int process_size;
    public:
        MMU(int page_table_size, int free_memory_size);
        void print_page_table();
        void allocate_space(int _process_id, int address_count);
        void initialize_free_memory(int size);
        int get_page_table_size();
        int evict();
        int empty_page();
        int get_free_memory_size();
        void reference(int _process_id, int address);
        int remove_me(int process_id);
        int find_page(int virtual_address_num);
        int random_address();
};
