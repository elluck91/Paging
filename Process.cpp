#include <thread>
using namespace std;
#include <chrono>
#include <random>
#include <iostream>
#include <mutex>
#include "Process.h"

std::mutex mtx3;

Process::Process(int _id, int _size, double _arrival_time, int _duration, double &_virtual_time) {
    process_id = _id;
    size = _size;
    arrival_time = _arrival_time;
    duration = _duration;
    virtual_time = &_virtual_time;
    remaining_time = _duration;
}
Process::Process() {
}
Process Process::create_process(int id, double &_virtual_time) {
    typedef chrono::system_clock clk;

    default_random_engine rand_engine(clk::now().time_since_epoch().count());
    typedef uniform_int_distribution<unsigned> random;
    random duration(1, 5); // in sec
    random pointer(0, 3);
    random arrival_time(0, 590);
    const unsigned memory_sizes[] = {5, 11, 17, 31};

    return Process(id,
        memory_sizes[pointer(rand_engine)],
        arrival_time(rand_engine) / 10.0,
        duration(rand_engine),
        _virtual_time);
}

void Process::print() {
    mtx3.lock();
    cout << "Process ID: " << process_id << " | size in MB: "
        << size << " | Arrival Time: " << arrival_time
        << " | Duration: " << duration << endl;
    mtx3.unlock();
}

void Process::set_process_id(int _id) {
    process_id = _id;
}

int Process::get_process_id() {
    return process_id;
}

void Process::set_size(int _size) {
    size = _size;
}

int Process::get_size() {
    return size;
}

void Process::set_arrival_time(double _arrival_time) {
    arrival_time = _arrival_time;
}

double Process::get_arrival_time() {
    return arrival_time;
}

void Process::set_duration(double _duration) {
    duration = _duration;
}

double Process::get_duration() {
    return duration;
}

void Process::run(MMU mmu) {
    typedef chrono::system_clock clk;
    default_random_engine rand_engine(clk::now().time_since_epoch().count());
    std::chrono::milliseconds timespan(100);
    typedef uniform_int_distribution<unsigned> random;
    random rand_memory(0, size-1); // in sec
    int exit_size = 0;

    while(remaining_time > 0.0) {
        // second argument should be a random number between 0 and size
        mmu.reference(process_id, rand_memory(rand_engine));
        this_thread::sleep_for(timespan);
        remaining_time -= 0.1;
    }
    exit_size = mmu.remove_me(process_id);
    cout << "Time: " << *virtual_time << "sec. | PID: " << process_id
        << " | Size Entered: 4 | Size Exited: " << exit_size
        << " | Service Duration: " << duration << endl;

    mmu.print_page_table();
}
