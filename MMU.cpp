#include <chrono>
#include <random>
#include <mutex>
#include "MMU.h"
std::mutex mtx;

using namespace std;
MMU::MMU(int page_table_size, int free_memory_size) {
    initialize_free_memory(free_memory_size);
    algo = -1;
    time_stamp = 0.0;
}

void MMU::initialize_free_memory(int size) {
    for (int i = 0; i < size; i++) {
        Memory temp;
        temp.set_process_id(-1);
        temp.set_address(-1);
        free_memory.push_back(temp);
    }
}

void MMU::print_page_table() {
    for (int i = 0; i < get_page_table_size(); i++) {
        cout << "Page Table Address: " << i
            << " | Process : " << page_table[i].get_process_id()
            << " | Address: " << page_table[i].get_address()
            << endl;
    }
}

void MMU::set_algo(int algorithm) {
    algo = algorithm;
}

int MMU::evict() {
    switch(algo) {
        // FIFO
        case 1:
			cout << "Page_queue size is: " << to_string(page_queue.size()) << endl;
			if (!page_queue.empty())
			{
				int page_index = page_queue.front();
				 //cout << "This page with index " << page_index << "will be evicted." << endl;
				page_queue.pop();
				 //cout << "Next page is " << page_queue.front() << endl;
				return page_index;
			}
            break;
        // LRU
        case 2:
            return lru_address();
            break;
        // LFU
        case 3:
            return least_frequently_used();
            break;
        // MFU
        case 4:
            return most_frequently_used();
            break;
        // random
        case 5:
        //    break;
        //default:
            return random_address();
            break;
    }
}

void MMU::allocate_space(int _process_id, int address_count) {
    Memory m;
    string temp_addr;
    for (int i = 0; i < address_count; i++) {
        m = free_memory.front();
        m.set_process_id(_process_id);
        temp_addr = to_string(_process_id) + "" + to_string(address_count % 4);
        m.set_address(stoi(temp_addr));
        free_memory.pop_front();
	// LRU_Cache
	if(algo == 2) {
		lru_cache.push_back(m);
	}
	int empty_index = empty_page();
	//cout << "This page with index " << empty_index << " has just been assigned. (Allocated)" << endl;
	page_table[empty_index] = m;
	if(algo == 1) {
		page_queue.push(empty_index);
	}
	//cout << "Has just added " << empty_index << " to page queue (Allocated)." << endl;
	if(algo == 3 || algo == 4) {
		page_use_frequency[stoi(temp_addr)] = 0;
	}

    }
}

int MMU::empty_page() {
    for (int i = 0; i < get_page_table_size(); i++) {
        if (page_table[i].get_address() == -1)
            return i;
    }
    return -1;
}

int MMU::get_page_table_size() {
    return sizeof(page_table) / sizeof(page_table[0]);
}
int MMU::nonzeros_count_pg() {
    int count = 0;
    for (int i = 0; i < get_page_table_size(); i++) {
        if (page_table[i].get_process_id() != -1)
            count++;
    }
    return count;
} 
int MMU::get_free_memory_size() {
    return free_memory.size();
}

void MMU::reference(int _process_id, int address) {
    std::lock_guard<std::mutex> lock(mtx);
    virtual_address_str = to_string(_process_id) + "" + to_string(address);
    virtual_address_num = stoi(virtual_address_str);
    // Referenced address found in page table
    if(find_page(virtual_address_num) != -1) {
        cout << "PID:" <<_process_id << " referencing: "
            << address << " | IN-MEMORY\n";
        std::deque<Memory>::iterator it = lru_cache.begin();

        // Update lru_cache so that page being referenced is moved to back of queue.
        if(algo == 2) {
          bool index_found = false;
          Memory most_recent_page;
          while(!index_found) { 
              if(it->get_address() == virtual_address_num) {
                  most_recent_page = *it;
                  lru_cache.erase(it);
                  index_found = true;
              }
              else {
                  it++;
              }
          }
          lru_cache.push_front(most_recent_page);
        }
        //increment page access frequency
	    //pair<int, double> pair_freq(page_use_frequency[virtual_address_num].first + 1, ;
	    if(algo == 3 || algo == 4) {
	           page_use_frequency[virtual_address_num] += 1;
	    }

        hit_count += 1;
        cout << "Hit count: " << hit_count << endl;

    }
    else {
        cout << "PID:" << _process_id << " referencing: " << address
            << " | PAGING!\n";
        // referenceed address not in page table
        // no free memory
        // paging required
        if (get_free_memory_size() > 0) {
		temp_mem = free_memory.front();
		temp_mem.set_process_id(_process_id);
		temp_mem.set_address(virtual_address_num);
		int empty_index = empty_page();
		//cout << "This page with index " << empty_index << "has just been assigned." << endl;
		page_table[empty_index] = temp_mem;
		free_memory.pop_front();
		// FIFO IMPLEMENTATION
		if(algo == 1) {
			page_queue.push(empty_index);
		}
		// LRU
		if(algo == 2) {
			lru_cache.push_back(temp_mem);
		}
		if(algo == 3 || algo == 4) {
			page_use_frequency[virtual_address_num] = 0;
		}
	} else { // I know it's redundant but logically it makes sense
		empty_page_index = evict();
		free_memory.push_back(page_table[empty_page_index]);
		temp_mem = free_memory.front();
		temp_mem.set_process_id(_process_id);
		temp_mem.set_address(virtual_address_num);
		page_table[empty_page_index] = temp_mem;
		free_memory.pop_front();
		// FIFO implementation
		if(algo == 1) {
			page_queue.push(empty_page_index);
		}
	    // LRU
	    if(algo == 2) {
		    lru_cache.push_back(temp_mem);
		}		
		   //LFU reset frequency
	       if(algo == 3 || algo == 4) {
	    	  page_use_frequency[virtual_address_num] = 0;
	       }

           successful_swap += 1;
           cout << "Successful swap count: " << successful_swap << endl;
        }
    }
}

int MMU::find_page(int virtual_address_num) {
    for(int i = 0; i < get_page_table_size(); i++) {
        if(page_table[i].get_address() == virtual_address_num) {
            return 1;
        }
    }
    return -1;
}


int MMU::get_page_index(int virtual_address_num) {
    for(int i = 0; i < get_page_table_size(); i++) {
        if(page_table[i].get_address() == virtual_address_num) {
            return i;
        }
    }
    return -1;
}

int MMU::remove_me(int process_id) {
    std::lock_guard<std::mutex> lock(mtx);

    cout << "Sum of memory before removal: " << (nonzeros_count_pg() + get_free_memory_size()) << endl;
    process_size = 0;
    for(int i = 0; i < get_page_table_size(); i++) {
        if(page_table[i].get_process_id() == process_id) {
            page_table[i].set_address(-1);
            page_table[i].set_process_id(-1);
            Memory temp;
            free_memory.push_back(temp);
            process_size++;
        }
    }
    cout << "Sum of memory after removal: " << (nonzeros_count_pg() + get_free_memory_size()) << endl;
   
    return process_size;
}

int MMU::random_address() {
    typedef chrono::system_clock clk;
    default_random_engine rand_engine(clk::now().time_since_epoch().count());
    typedef uniform_int_distribution<unsigned> random;
    random rand_memory(0, get_page_table_size()-1);
    
    int rand_mem = rand_memory(rand_engine);
    // cout << "Random Evicting " <<
    //         page_table[rand_mem].get_address() <<
    //         " of Process " <<
    //         to_string(page_table[rand_mem].get_process_id())
    // << endl;

    return rand_mem;
}


// LRU eviction algorithm
int MMU::lru_address() {
    //  cout << "lru_cache is: " << endl;
    //  std::deque<Memory>::iterator it = lru_cache.begin();
    //  while(it != lru_cache.end()) {
    //      cout << "PID: " + to_string(it->get_process_id()) + "ADDRESS: " + to_string(it->get_address()) << endl;
    //      it++;
    //  }
    Memory m = lru_cache.front();
    lru_cache.pop_front();
    string virtual_address_string = to_string(m.get_process_id()) + "" + to_string(m.get_address());
    int virtual_address_number = stoi(virtual_address_string);
    // cout << "LRU Evicting " <<
    //         virtual_address_string <<
    //         " of Process " <<
    //         to_string(m.get_process_id())
    // << endl;

    return get_page_index(m.get_address());
}

//LFU algorithm
int MMU::least_frequently_used() {
// 	for(std::map<int, int>::iterator it = page_use_frequency.begin(); it != page_use_frequency.end(); it++) {
// 		if(it->second != 0) {
// 			cout << it->first << " => " << it->second  << " NUM " << k << endl;
// 		}
// 	}
	string virt_addr_str = to_string(page_table[0].get_address());
	int virt_addr_num = stoi(virt_addr_str);
	int current_page_address = virt_addr_num; // Holds address of first element in page_table
	int min_temp = page_use_frequency.at(current_page_address);
	int lfu_virt_addr;
	int i, j;
	bool found = false;
	for(i = 1; i < get_page_table_size(); i++) {
		virt_addr_str = to_string(page_table[i].get_address());
		virt_addr_num = stoi(virt_addr_str);
		current_page_address = virt_addr_num;
		// cout << current_page_address << " => " << page_use_frequency.at(current_page_address) << endl;
		if(min_temp > page_use_frequency.at(current_page_address)) {
			min_temp = page_use_frequency.at(current_page_address);
		}
	    lfu_virt_addr = i;	 
	}

    //j = 0;
    // while (!found && j < page_use_frequency.size()) {
    //     virt_addr_str = to_string(page_table[j].get_process_id()) + "" + to_string(page_table[j].get_address());
    //     virt_addr_num = stoi(virt_addr_str);
    //     if (min_temp == page_use_frequency.at(virt_addr_num)) {
    //         lfu_virt_addr = j;
    //         found = true;
    //     }
    //     j++;
    // }

	return lfu_virt_addr;
	
}

//MFU algorithm
int MMU::most_frequently_used() {
    string virt_addr_str = to_string(page_table[0].get_address());
    int virt_addr_num = stoi(virt_addr_str);
    int max_temp = page_use_frequency.at(virt_addr_num);
    int mfu_virt_addr;
    int i;
    bool found = false;
    for(i = 1; i < get_page_table_size(); i++) {
        virt_addr_str = to_string(page_table[i].get_address());
        virt_addr_num = stoi(virt_addr_str);
        if(max_temp < page_use_frequency.at(virt_addr_num)) {
            max_temp = page_use_frequency.at(virt_addr_num);
        }
        mfu_virt_addr = i;
    }

   //  j = 0;
   //  while (!found) {
   //      virt_addr_str = to_string(page_table[j].get_process_id()) + "" + to_string(page_table[j].get_address());
   //      virt_addr_num = stoi(virt_addr_str);
   //      if (max_temp == page_use_frequency.at(virt_addr_num)) {
   //          mfu_virt_addr = j;
   //          found = true;
   //      }
   //      j++;
   //  }

    return mfu_virt_addr;
    
}
