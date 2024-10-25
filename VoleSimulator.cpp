#include "VoleSimulator.hpp"
string decimalToHex(int decimal) {
    stringstream ss;
    ss << hex << decimal; 
    return ss.str();            
}

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
    regex pat2("40[0123456789ABCDEF][0123456789ABCDEF]");
    regex pat3("[12356789B][0123456789ABCDEF][0123456789ABCDEF][0123456789ABCDEF]");
    smatch m;

    if (regex_match(instruction, m, pat1) || regex_match(instruction, m, pat2) || regex_match(instruction, m, pat3)){
        return instruction;
    }

    else {
        return "";
    }
}

string Memory::getScreen(){
    return memory_cells[0];
}

string Memory::getcell(string cell){
    return memory_cells[stoi(cell, nullptr, 16)];
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

string Register::getcell(string cell){
    return reg_cells[stoi(cell, nullptr, 16)];
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

    else{
        throw runtime_error("Invalid Input");
    }
}

void Machine::execute_IR(){
    if (IR[0] == '1'){
        string register_cell = string(1, IR[1]);
        string memory_cell = string(1, IR[2]) + string(1, IR[3]);
        reg.alter_cell(register_cell, main_memory.getcell(memory_cell));
        PC = decimalToHex(stoi(PC, nullptr, 16) + 2);
    }

    else if (IR[0] == '2'){
        string register_cell = string(1, IR[1]);
        string bit_pattern = string(1, IR[2]) + string(1, IR[3]);
        reg.alter_cell(register_cell, bit_pattern);
        PC = decimalToHex(stoi(PC, nullptr, 16) + 2);
    }

    else if (IR[0] == '3'){
        string register_cell = string(1, IR[1]);
        string memory_cell = string(1, IR[2]) + string(1, IR[3]);
        main_memory.alter_cell(memory_cell, reg.getcell(register_cell));
        PC = decimalToHex(stoi(PC, nullptr, 16) + 2);
    }

    else if (IR[0] == '4'){
        string regcell1 = string(1, IR[2]);
        string regcell2 = string(1, IR[3]);
        reg.alter_cell(regcell2, reg.getcell(regcell1));
        PC = decimalToHex(stoi(PC, nullptr, 16) + 2);
    }

    else if (IR[0] == '5' || IR[0] == '6'){
        string regcell1 = string(1, IR[2]);
        string regcell2 = string(1, IR[3]);
        string targetreg = string(1, IR[1]);
        reg.alter_cell(targetreg, decimalToHex(stoi(regcell1, nullptr, 16) + stoi(regcell2, nullptr, 16)));
        PC = decimalToHex(stoi(PC, nullptr, 16) + 2);
    }

    else if (IR[0] == 'B'){
        string register_cell = string(1, IR[1]);
        string memory_cell = string(1, IR[2]) + string(1, IR[3]);
        if(reg.getcell(register_cell) == reg.getcell("0")){
            PC = decimalToHex(stoi(memory_cell, nullptr, 16));
        }
        else{
            PC = decimalToHex(stoi(PC, nullptr, 16) + 2);
        }
    }

    else if (IR[0] == 'C'){
        end = true;
    }


    if (stoi(PC, nullptr, 16) <=254){
        IR = main_memory.getInstruction(PC);
    }

    else{
        PC = "";
        IR = "";
        end = true;
    }
}