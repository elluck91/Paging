#include <string>

using namespace std;

#include "Process.h"

class Memory {
    private:
        int address;
        Process process;
    public:
        Memory();
        Memory(int _address, Process _process);
        int get_address();
        void set_address(int _address);
        void set_process(Process _process);
        Process get_process();
};