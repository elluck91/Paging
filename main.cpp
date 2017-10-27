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

int main() {
    // define variables
    unsigned job_size;
    list<Process> job_queue;
    list<Memory> free_memory;
    list<Memory> used_memory;
    string user_input;

    // user input
    cout << "Enter number of jobs to create: ";
    getline(cin, user_input);
    istringstream(user_input) >> job_size;

    job_queue = queue_generator(job_size);
}

list<Process> queue_generator(unsigned job_size) {
    vector<Process> jobs;

    for (int i = 0; i < job_size; i++) {
        job.push_back(Process::create_process(i));
    }

    return jobs;
}

