#ifndef SIMULATOR
#define SIMULATOR

#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <bitset>
#include <cmath>
using namespace std;

class Arithmetic {
    public:
    int Hextoint(string hex);
    string inttoHex(int decimal);
    string Hextobin(string hex);
    string bintoHex(string bin);
    float Hextofloat(string hex);
    string floattoHex(float flt);
};

class Register{
    private:
    vector<string> reg_cells = vector<string> (16, "00");
    Arithmetic ALU;
    public:
    void alter_cell(string cell, string new_value);
    string getcell(string cell);
    friend ostream& operator<<(ostream& o, Register& r);
};

class Memory{
    private:
    vector<string> memory_cells = vector<string> (256, "00");
    Arithmetic ALU;
    public:
    void load_cells(string filename, string start_cell = "10");
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
    Arithmetic ALU;
    string PC;
    string IR;
    public:
    bool end = false;
    Machine();
    void menu();
    void execute_IR();
};





#endif