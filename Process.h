#include <string>
#include <random>
using namespace std;

class Process {
    private:
        int process_id;
        int size;
        double arrival_time;
        int duration;
    public:
        Process();
        Process(int _id, int s, double at, int d);
        void print();
        static Process create_process(int id);
        void set_process_id(int _id);
        int get_process_id();
        void set_size(int _size);
        int get_size();
        void set_arrival_time(double _arrival_time) ;
        double get_arrival_time();
        void set_duration(double _duration);
        double get_duration();
};
