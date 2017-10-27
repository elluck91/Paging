#include <list>
#include "Memory.h"

class Process {
    private:
        string process_name;
        unsigned size;
        unsigned arrival_time;
        unsigned duration;
        Memory local_space[4];
    public:
        Process(
            string _name,
            unsigned _size,
            unsigned _arrival_time,
            unsigned _duration);
};