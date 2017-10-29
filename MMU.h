#include <string>

using namespace std;

#include "Memory.h"

class MMU {
    private:
        Memory page_table[];
        list<Memory> free_memory;
        double time_stamp;
        string process_to_evict;
        int algo;
    public:
        MMU();
        MMU(int page_table_size, int free_memory_size);
        void print_page_table();
        void allocate(string process_name, int address_count);
        void initialize_free_memory(int size);
        int get_page_table_size();
        int evict();
        int empty_page();
};