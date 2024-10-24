#ifndef SIMULATOR
#define SIMULATOR

#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Machine{
    private:
    Memory main_memory;
    Register reg;
    string PC;
    string IR;
    public:
    void menu();
    void execute_IR();
};

class Memory{
    private:
    vector<string> memory_cells(256);
    public:
    void load_cells(string filename);
    void alter_cell(string cell);
};

class Register{
    private:
    vector<string> reg_cells(16);
    public:
    void alter_cell(string cell);
};

#endif