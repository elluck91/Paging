#include "MMU.h"
class Process {
    private:
        int process_id;
        int size;
        double arrival_time;
        int duration;
        double *virtual_time;
        double remaining_time;
    public:
        Process();
        Process(int _id, int s, double at, int d, double &_virtual_time);
        void print();
        static Process create_process(int id, double &_virtual_time);
        void set_process_id(int _id);
        int get_process_id();
        void set_size(int _size);
        int get_size();
        void set_arrival_time(double _arrival_time) ;
        double get_arrival_time();
        void set_duration(double _duration);
        double get_duration();
        void run(MMU mmu);
        double get_virtual_time();
};
