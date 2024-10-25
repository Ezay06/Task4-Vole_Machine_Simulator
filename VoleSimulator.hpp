#ifndef SIMULATOR
#define SIMULATOR

#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

string decimalToHex(int decimal);

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
    string getcell(string cell);
    void alter_cell(string cell, string new_value);
    friend ostream& operator<<(ostream& o, Memory& m);
};

class Register{
    private:
    vector<string> reg_cells{16};
    public:
    void alter_cell(string cell, string new_value);
    string getcell(string cell);
    friend ostream& operator<<(ostream& o, Register& r);
};

#endif