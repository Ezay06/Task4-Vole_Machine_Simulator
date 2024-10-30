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
    int memory_cell = ALU.Hextoint(cell);
    memory_cells[memory_cell] = new_value;
}

string Memory::getInstruction(string PC){
    int cell = ALU.Hextoint(PC);
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
    return memory_cells[ALU.Hextoint(cell)];
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
    int memory_cell = ALU.Hextoint(cell);
    reg_cells[memory_cell] = new_value;
}

string Register::getcell(string cell){
    return reg_cells[ALU.Hextoint(cell)];
}

ostream& operator<<(ostream& o, Register& r){
    for(int i = 0; i < 16; i++){
        o << "R" << i << " " << r.reg_cells[i] << endl;
    }
    return o;
}


int Arithmetic::Hextoint(string hex){
    return stoi(hex, nullptr, 16);
}

string Arithmetic::inttoHex(int decimal){
    stringstream ss;
    ss << hex << decimal; 
    string hexa = ss.str();
    if (hexa.size() == 1){
        return "0" + hexa;
    }
    else{
        return hexa;
    } 
}

string Arithmetic::Hextobin(string hex){
    int dec = Hextoint(hex);
    bitset<8> binary(dec);
    return binary.to_string();
}

string Arithmetic::bintoHex(string bin){
    int dec = stoi(bin, nullptr, 2);
    return inttoHex(dec);
}

float Arithmetic::Hextofloat(string hex){
    string binary = Hextobin(hex);
    int sign = pow(-1, stoi(hex.substr(0, 1)));
    int exponent = stoi(hex.substr(1, 3), nullptr, 2) - 4;
    int mantissa = stoi(hex.substr(4, 4), nullptr, 2);
    float result = sign * pow(2, exponent) * (mantissa / 16.0f);
    return result;
}

string Arithmetic::floattoHex(float flt){
    if (flt == 0.0){
        return inttoHex(0);
    }

    string sign = (flt < 0) ? "1" : "0";
    int exponent = 0;
    string mantissa;
    if (abs(flt) < 1){
        while(true){
            flt *= 2;
            if (abs(flt) > 1){
                flt *= 0.5f;
                break;
            } 

            else {
                exponent--;
            }
        }
    }

    else {
        while(true){
            flt *= 0.5f;
            exponent++;
            if (abs(flt) < 1){
                break;
            }
        }
    }

    bitset<3> power(exponent + 4);
    string exp = power.to_string();

    mantissa = to_string(abs(flt));
    mantissa.erase(0);
    mantissa.erase(0);
    bitset<4> man(stoi(mantissa) * 16);
    mantissa = man.to_string();

    string binary = sign + exp + mantissa;
    return bintoHex(binary);

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
        PC = ALU.inttoHex(ALU.Hextoint(PC) + 2);
    }

    else if (IR[0] == '2'){
        string register_cell = string(1, IR[1]);
        string bit_pattern = string(1, IR[2]) + string(1, IR[3]);
        reg.alter_cell(register_cell, bit_pattern);
        PC = ALU.inttoHex(ALU.Hextoint(PC) + 2);
    }

    else if (IR[0] == '3'){
        string register_cell = string(1, IR[1]);
        string memory_cell = string(1, IR[2]) + string(1, IR[3]);
        main_memory.alter_cell(memory_cell, reg.getcell(register_cell));
        PC = ALU.inttoHex(ALU.Hextoint(PC) + 2);
    }

    else if (IR[0] == '4'){
        string regcell1 = string(1, IR[2]);
        string regcell2 = string(1, IR[3]);
        reg.alter_cell(regcell2, reg.getcell(regcell1));
        PC = ALU.inttoHex(ALU.Hextoint(PC) + 2);
    }

    else if (IR[0] == '5'){
        string regcell1 = string(1, IR[2]);
        string regcell2 = string(1, IR[3]);
        string targetreg = string(1, IR[1]);
        reg.alter_cell(targetreg, ALU.inttoHex(ALU.Hextoint(regcell1) + ALU.Hextoint(regcell2)));
        PC = decimalToHex(stoi(PC, nullptr, 16) + 2);
    }

    else if (IR[0] == '6'){
        string regcell1 = string(1, IR[2]);
        string regcell2 = string(1, IR[3]);
        string targetreg = string(1, IR[1]);
        reg.alter_cell(targetreg, ALU.floattoHex(ALU.Hextofloat(regcell1) + ALU.Hextofloat(regcell2)));
        PC = decimalToHex(stoi(PC, nullptr, 16) + 2);
    }

    else if (IR[0] == 'B'){
        string register_cell = string(1, IR[1]);
        string memory_cell = string(1, IR[2]) + string(1, IR[3]);
        if(reg.getcell(register_cell) == reg.getcell("0")){
            PC = ALU.inttoHex(ALU.Hextoint(memory_cell));
        }
        else{
            PC = ALU.inttoHex(ALU.Hextoint(PC) + 2);
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