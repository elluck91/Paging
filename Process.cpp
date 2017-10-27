#include "Process.h"

using namespace std;

Process::Process(string _name, unsigned _size, unsigned _arrival_time, unsigned _duration) {
    process_name = _name;
    size = _size;
    arrival_time = _arrival_time;
    duration = _duration;
}

Process::create_process(int id) {
    typedef uniform_int_distribution<unsigned> random;
    default_ranom_engine rand_engine(clk::now().time_since_epoch().count());
    random duration(1, 5);
    random pointer(1, 4);
    const unsigned memory_sizes[] = {5, 11, 17, 31};

    return Process("PID" + to_string(id), memory_sizes[])

}