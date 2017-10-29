#include <string>
#include <iostream>
#include <list>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;
#include "MMU.h"

vector<Process> queue_generator(unsigned job_size);
int compare(Process one, Process two);

int main() {
    const int page_table_size = 100;
    const int free_memory_size = 100;

    // define variables
    unsigned job_size;
    vector<Process> jobs_queue;
    string user_input;

    // user input
    cout << "Enter number of jobs to create: ";
    getline(cin, user_input);
    istringstream(user_input) >> job_size;

    jobs_queue = queue_generator(job_size);

    MMU mmu(page_table_size, free_memory_size);
    for (auto &job : jobs_queue) {
        if (mmu.get_free_memory_size() != 0)
            mmu.allocate_space(job, 4);
        else
            break;
    }
    
    mmu.print_page_table();
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
     return (one.get_arrival_time() - two.get_arrival_time()) < 0;
}
