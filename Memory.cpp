#include <string>
#include <cstddef>

#include "Memory.h"

Memory::Memory(int _address, Process _process) {
        address = _address;
        process = _process;
}

Memory::Memory() {
    address = -1;
}

int Memory::get_address() {
    return address;
}

void Memory::set_address(int _address) {
    address = _address;
}

void Memory::set_process(Process _process) {
    process = _process;
}

Process Memory::get_process() {
    return process;
}