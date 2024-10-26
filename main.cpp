#include "VoleSimulator.hpp"

int main(){
    Machine Volemachine;
    while(!Volemachine.end){
        try{
            Volemachine.menu();
        }
        catch(...){
            cout << "Invalid input. Please enter an option between 1-6.";
        }    
    }
}