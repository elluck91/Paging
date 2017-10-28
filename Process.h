#include <string>
#include <random>
using namespace std;

class Process {
    public:
        string process_name;
        unsigned size;
        double arrival_time;
        double duration;
    //public:
        Process();
        Process(string name, unsigned s, double at, double d);
        void print();
        static Process create_process(int id);
};