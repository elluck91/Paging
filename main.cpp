#include <thread>
#include <string>
#include <iostream>
#include <list>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cassert>
using namespace std;
#include "Process.h"

vector<Process> queue_generator(unsigned job_size, double &_virtual_time);
int compare(Process one, Process two);
void pop_front(vector<Process> &vec);

int main() {
    vector<std::thread> ts;
    const int page_table_size = 100;
    const int free_memory_size = 100;
    const int minimum_page_count = 4;
    const double last_time = 59.9;
    std::chrono::milliseconds timespan(100);
    
    // at 100msec intervals processes will reference in addresses
    double virtual_time = 0.0;

    // define variables
    unsigned job_size;
    vector<Process> jobs_queue;
    vector<Process> jobs_complete;
    string user_input;
    
    // initialize MMU
    MMU mmu(page_table_size, free_memory_size);
    mmu.set_algo(4);

    // user input
    cout << "Enter number of jobs to create: ";
    getline(cin, user_input);
    istringstream(user_input) >> job_size;

    jobs_queue = queue_generator(job_size, virtual_time);
    Process temp;

    cout << "Jobs_queue size: " << jobs_queue.size() << endl; 
    // a little tricky to understand, but I need to store pointer to
    // allocated jobs in order to start them simultaneously later
    int i = 0;
    while(i < job_size) {
        if (mmu.get_free_memory_size() != 0) {
            temp = jobs_queue[i];
            mmu.allocate_space(temp.get_process_id(), 4);
            i++;
        }
        else
            break;
    }
/*
    i = 0;
    while(i < job_size) {
        mmu.remove_me(jobs_queue[i].get_process_id());
        i++;
    }
*/

    //mmu.print_page_table();
    for(int n = 0; n < i; n++) {
        cout << "PID" << n  << endl;
        ts.push_back(std::thread(&Process::run, jobs_queue[n], ref(mmu)));
        jobs_complete.push_back(jobs_queue[n]);
    } 
   
    for(int n = 0; n < i; n++)
       pop_front(jobs_queue); 

    while (virtual_time < last_time) {
        if (mmu.get_free_memory_size() >= minimum_page_count && !jobs_queue.empty()) {
            temp = jobs_queue.front();
            pop_front(jobs_queue);
            mmu.allocate_space(temp.get_process_id(), 4);
            ts.push_back(std::thread(&Process::run, temp, ref(mmu)));
        }

        std::this_thread::sleep_for(timespan);
        virtual_time += 0.1;
    }

    for (int z = 0; z < ts.size(); z++) {
        ts[z].join();
    }
  
    return 0;
}

vector<Process> queue_generator(unsigned job_size, double &_virtual_time) {
    vector<Process> jobs;

    for (int i = 0; i < job_size; i++) {
        jobs.push_back(Process::create_process(i, _virtual_time));
    }
    sort(jobs.begin(), jobs.end(), compare);
/*
    for (int n = 0; n < job_size; n++)
        jobs[n].print();
*/
    return jobs;
}

int compare(Process one, Process two) {
     return (one.get_arrival_time() - two.get_arrival_time()) < 0;
}

void pop_front(vector<Process>& vec) {
    //assert(!vec.empty());
    vec.erase(vec.begin());
}
