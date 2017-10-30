using namespace std;
#include "Memory.h"

Memory::Memory(int _address, int _process_id) {
        address = _address;
        process_id = _process_id;
}

Memory::Memory() {
    address = -1;
    process_id = -1;
}

int Memory::get_address() {
    return address;
}

void Memory::set_address(int _address) {
    address = _address;
}

void Memory::set_process_id(int _process_id) {
    process_id = _process_id;
}

int Memory::get_process_id() {
    return process_id;
}
