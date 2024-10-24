#include "VoleSimulator.hpp"

void Memory::load_cells(string filename){
    string instruct;
    ifstream instructions(filename);
    for (int i = 0; i < memory_cells.size(); i = i + 2){
        instructions >> instruct;
        memory_cells[i] = string(1, instruct[0]) + string(1, instruct[1]);
        memory_cells[i + 1] = string(1, instruct[2]) + string(1, instruct[3]);
    }
}

void Memory::alter_cell(string cell, string new_value){
    int memory_cell = stoi(cell, nullptr, 16);
    memory_cells[memory_cell] = new_value;
}

string Memory::getInstruction(string PC){
    int cell = stoi(PC, nullptr, 16);
    string instruction = memory_cells[cell] + memory_cells[cell + 1];
    regex pat1("C000");
    regex pat2("3[0123456789ABCDEF]00");
    regex pat3("40[0123456789ABCDEF][0123456789ABCDEF]");
    regex pat4("[1256789B][0123456789ABCDEF][0123456789ABCDEF][0123456789ABCDEF]");
    smatch m;

    if (regex_match(instruction, m, pat1) || regex_match(instruction, m, pat2) || regex_match(instruction, m, pat3) || regex_match(instruction, m, pat4)){
        return instruction;
    }

    else {
        return "";
    }
}

string Memory::getScreen(){
    return memory_cells[0];
}

ostream& operator<<(ostream& o, Memory& m){
    for (int i = 0; i < 16; i++){
        o << "   " << i; 
    }

    cout << endl;
    for (int i = 0; i < 16; i++){
        cout << i;
        for (int j = 0; j < 16; j++){
            cout << "  " << m.memory_cells[i * j];
        }
        o << endl;
    }
    return o;
}


void Register::alter_cell(string cell, string new_value){
    int memory_cell = stoi(cell, nullptr, 16);
    reg_cells[memory_cell] = new_value;
}

ostream& operator<<(ostream& o, Register& r){
    for(int i = 0; i < 16; i++){
        o << "R" << i << " " << r.reg_cells[i] << endl;
    }
    return o;
}


Machine::Machine(){
    string filename;
    cout << "Welcome to the Vole Machine Simulator. Please enter the path of the instruction file: ";
    cin >> filename;
    main_memory.load_cells(filename);
    PC = "00";
    IR = main_memory.getInstruction(PC);
}

void Machine::menu(){
    int option;
    cout << "1-Display memory\n2-Display registers\n3-Display PC\n4-Display IR\n5-Display screen\n6-Execute IR" << endl;
    cout << "Choose one of the previous options(1-6): ";
    cin >> option;
    if (option == 1){
        cout << main_memory;
    }

    else if (option == 2){
        cout << reg;
    }

    else if (option == 3){
        cout << PC;
    }

    else if (option == 4){
        cout << IR;
    }

    else if (option == 5){
        cout << main_memory.getScreen();
    }

    else if (option == 6){
        execute_IR();
    }
}