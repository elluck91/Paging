#include <string>
#include <iostream>
#include <list>
#include <sstream>
#include <vector>
#include <random>
#include <ctime>

using namespace std;

#include "Process.h"
#include 
list<Process> queue_generator(unsigned job_size);
typedef uniform_int_distribution<unsigned> random;

int main() {
    // define variables
    unsigned job_size;
    list<Process> job_queue;
    list<Memory> free_memory;
    list<Memory> used_memory;
    string user_input;
    default_ranom_engine rand_engine(clk::now().time_since_epoch().count());
    random duration(1, 5);
    random pointer(1, 4);
    const int memory_sizes[] = {5, 11, 17, 31};

    // user input
    cout << "Enter number of jobs to create: ";
    getline(cin, user_input);
    istringstream(user_input) >> job_size;
    
    job_queue = queue_generator(job_size);
}

list<Process> queue_generator(unsigned job_size) {
    vector<Process> jobs;
    
    for (int i = 0; i < job_size; i++) {
        job.push_back({
            // Process
        })
    }
    
    return jobs;
}

