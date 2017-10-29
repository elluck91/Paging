#include <list>
#include <vector>
#include <string>
#include <iostream>
#include <cstddef>

#include "MMU.h"

MMU::MMU() {
    MMU(0, 0);
}

MMU::MMU(int page_table_size, int free_memory_size) {
    page_table[page_table_size] = {};
    initialize_free_memory(free_memory_size);
    time_stamp = 0.0;
}

void MMU::initialize_free_memory(int size) {
    for (int i = 0; i < size; i++) {
        Memory temp;
        free_memory.push_back(temp);
    }
}

void MMU::print_page_table() {
    for (int i = 0; i < get_page_table_size(); i++) {
        cout << "Page Table Address: " << i
            << " | Process : " << page_table[i].get_process().get_process_id()
            << " | Address: " << page_table[i].get_address()
            << endl;
    }
}

int MMU::evict() {
    switch(algo) {
        // FIFO
        case 1:
            break;
        // LRU
        case 2:
            break;
        // LFU
        case 3:
            break;
        // MFU
        case 4:
            break;
        // random
        case 5:
            break;
        default:
            break;
    }
}

void MMU::allocate_space(Process _process, int address_count) {
    Memory m;
    string temp_addr;
    for (int i = 0; i < address_count; i++) {
        m = free_memory.front();
        m.set_process(_process);
        temp_addr = to_string(_process.get_process_id()) + "" + to_string(m.get_address());
        m.set_address(stoi(temp_addr));
        free_memory.pop_front();
        page_table[empty_page()] = m;
    }

}

int MMU::empty_page() {
    for (int i = 0; i < get_page_table_size(); i++) {
        if (page_table[i].get_address() == -1)
            return i;
    }
    return -1;
}

int MMU::get_page_table_size() {
    return sizeof(page_table) / sizeof(Memory);
}