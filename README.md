Swapping and Paging
The purpose of this assignment is to explore the various memory management algorithms for swapping and paging. Write a series of small simulation programs in Java, C, or C++ (your choice). Generate simulated Processes <Process Name, Process size in pages, arrival time, Service duration> using random number generator.

Execution:
The algo can be set in main.cpp by calling the set_algo(int algorithm) where 1 = FIFO, 2 = LRU, 3 = LFU, 4 = MFU, 5 = Random. Default is random (5).

g++ *.cpp -o main
./main
