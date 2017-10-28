#include <string>
#include <iostream>
#include <list>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

#include "Process.h"
#include "Memory.h"


vector<Process> queue_generator(unsigned job_size);
int compare(Process one, Process two);

int main() {
    // define variables
    unsigned job_size;
    vector<Process> job_queue;
    list<Memory> free_memory;
    list<Memory> page_table;
    string user_input;

    // user input
    cout << "Enter number of jobs to create: ";
    getline(cin, user_input);
    istringstream(user_input) >> job_size;

    job_queue = queue_generator(job_size);



    return 0;
}

vector<Process> queue_generator(unsigned job_size) {
    vector<Process> jobs;

    for (int i = 0; i < job_size; i++) {
        jobs.push_back(Process::create_process(i));
    }

    sort(jobs.begin(), jobs.end(), compare);

    for (int n = 0; n < job_size; n++)
        jobs[n].print();

    return jobs;
}

int compare(Process one, Process two) {
     return (one.arrival_time - two.arrival_time) < 0;
}
