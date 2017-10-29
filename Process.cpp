#include <string>
#include <random>
#include <iostream>
#include <random>
#include <ctime>
#include <chrono>

#include "Process.h"

Process::Process(int _id, int _size, double _arrival_time, int _duration) {
    process_id = _id;
    size = _size;
    arrival_time = _arrival_time;
    duration = _duration;
}

Process::Process() {
    Process(0, 0, 0.0, 0);
}

Process Process::create_process(int id) {
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
        duration(rand_engine));
}

void Process::print() {
    cout << "Process ID: " << process_id << " | size in MB: "
        << size << " | Arrival Time: " << arrival_time
        << " | Duration: " << duration << endl;
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
