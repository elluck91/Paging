class Memory {
    private:
        int address = -1;
        int process_id = -1;
    public:
        Memory();
        Memory(int _address, int _process_id);
        int get_address();
        void set_address(int _address);
        void set_process_id(int _proces_id);
        int get_process_id();
};


