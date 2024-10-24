#include "VoleSimulator.hpp"

void Memory::load_cells(string filename){
    string instruct;
    ifstream instructions(filename);
    regex pat1("3[0123456789ABCDEF]00");
    regex pat2("40[0123456789ABCDEF][0123456789ABCDEF]");
    regex pat3("[1256789B][0123456789ABCDEF][0123456789ABCDEF][0123456789ABCDEF]");
    smatch m;
    for (int i = 0; i < memory_cells.size(); i = i + 2){
        instructions >> instruct;
        if (instruct[0] == 'C' && instruct == "C000"){
            memory_cells[i] = "C0";
            memory_cells[i + 1] = "00";
        }

        else if (regex_match(instruct, m, pat1) || regex_match(instruct, m, pat2) || regex_match(instruct, m, pat3)){
            memory_cells[i] = string(1, instruct[0]) + string(1, instruct[1]);
            memory_cells[i + 1] = string(1, instruct[2]) + string(1, instruct[3]);
        }

        else {
            continue;
        }
    }
}

void Memory::alter_cell(string cell, string new_value){
    int memory_cell = stoi(cell, nullptr, 16);
    memory_cells[memory_cell] = new_value;
}

ostream& operator<<(ostream& o, Memory& m){
    for (int i = 0; i < 16; i++){
        cout << "   " << i; 
    }

    cout << endl;
    for (int i = 0; i < 16; i++){
        cout << i;
        for (int j = 0; j < 16; j++){
            cout << "  " << m.memory_cells[i * j];
        }
        cout << endl;
    }
}
