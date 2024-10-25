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

class Register{
    private:
    vector<string> reg_cells = vector<string> (16);
    public:
    void alter_cell(string cell, string new_value);
    string getcell(string cell);
    friend ostream& operator<<(ostream& o, Register& r);
};

class Memory{
    private:
    vector<string> memory_cells = vector<string> (256);
    public:
    void load_cells(string filename);
    string getInstruction(string PC);
    string getScreen();
    string getcell(string cell);
    void alter_cell(string cell, string new_value);
    friend ostream& operator<<(ostream& o, Memory& m);
};

class Machine{
    private:
    Memory main_memory;
    Register reg;
    string PC;
    string IR;
    public:
    bool end = false;
    Machine();
    void menu();
    void execute_IR();
};





#endif