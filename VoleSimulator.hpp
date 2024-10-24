#ifndef SIMULATOR
#define SIMULATOR

#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <fstream>
using namespace std;

class Machine{
    private:
    Memory main_memory;
    Register reg;
    string PC;
    string IR;
    public:
    Machine();
    void menu();
    void execute_IR();
};

class Memory{
    private:
    vector<string> memory_cells{256};
    public:
    void load_cells(string filename);
    string getInstruction(string PC);
    string getScreen();
    void alter_cell(string cell, string new_value);
    friend ostream& operator<<(ostream& o, Memory& m);
};

class Register{
    private:
    vector<string> reg_cells{16};
    public:
    void alter_cell(string cell, string new_value);
    friend ostream& operator<<(ostream& o, Register& r);
};

#endif