// © copyright 2015 Olivia James. All Rights Reserved.
// Author: Olivia James
// Date: 11-18-2015
// Contact: leslie.james@live.lagcc.cuny.edu
// Description: Lab 4 - My CPU

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <stdlib.h>

using namespace std;

string filename_prompt();
map <string, int> translation;                  // Map relating each mnemonic to a number
map <string, int> ::iterator valid_entry;       // Map iterator
void create_translation();
void execute_instructions(int opcode);
void process_file();
void additional_instructions ();
void display_menu();


int ACC, PC=0, opcode, operand, result, new_operand;         // Variables to hold values for accumulator, program counter
                                                // opcode, operand, result of string converted to int
vector<int> RAM;                                // Vector to hold opcodes and operands
string filename, entry, user_added_opcode;                         // Variables to hold filename and instructions to be interpreted by translator


int main()
{

    create_translation();                       // Translate mnemonics to numbers for execution
    process_file();
    additional_instructions ();

    return 0;
}


string filename_prompt(){
    string textfile;

    cout << "Enter the filename: " << endl;
    cin >> textfile;
    return textfile;
}


void create_translation(){
    translation ["CLR"] = 0;
    translation ["LOAD"] = 1;
    translation ["ADD"] = 2;
    translation ["SUB"] = 3;
    translation ["MUL"] = 4;
    translation ["DIV"] = 5;
    translation ["OUT"] = 6;
    translation ["JMP"] = 7;
    translation ["BNZ"] = 8;
    translation ["HALT"] = 9;
}

//Function to execute instructions based on mnemonics
void execute_instructions(int opcode){
    switch(opcode){
        case 0:                         // Clear ACC to zero
            ACC = 0;
            PC++;
            break;
        case 1:                         // DEFINE THIS!

            PC++;
            break;
        case 2:                         // Add to ACC
            operand = RAM[++PC];
            ACC += operand;
            PC++;
            break;
        case 3:                         // Subtract from ACC
            operand = RAM[++PC];
            ACC -= operand;
            PC++;
            break;
        case 4:                         // Multiply ACC by some number
            operand = RAM[++PC];
            ACC *= operand;
            PC++;
            break;
        case 5:                         // Divide ACC by some number
            operand = RAM[++PC];
            if (operand == 0)
                cout << "Cannot divide by zero" <<endl;     // Error if trying to divide by zero
            else{
                ACC /= operand;
                PC++;
            }
            break;
        case 6:                         // Output results
            cout << "The result of your instructions is "<< ACC << endl;
            PC++;
            break;
        case 7:                         // Jump to instruction
            operand = RAM[++PC];
            PC = operand;
            break;
        case 8:                         //
            operand = RAM[++PC];
            if ( ACC != 0 )
                PC = operand;
            else
                PC++;
            break;
        case 9:                         // Halt program
            cout << "Program halted" << endl;
            break;
    }
}

void process_file(){

    filename = filename_prompt();               // Prompt for name of file
    ifstream myfile(filename.c_str());

    if (myfile.is_open()){
        while (myfile >> entry && !myfile.eof()){
            valid_entry = translation.find(entry);
            if (valid_entry == translation.end()){
                istringstream(entry) >> result;
                RAM.push_back(result);
                }
            else
                RAM.push_back(valid_entry->second);
        }

        for(int i=0; i<RAM.size(); i++){
            // Fetch cycle
            opcode = RAM[PC];
            execute_instructions(opcode);
        }
    }
    else {
            cout << "Error in opening your file, try again." << endl;
    }

    myfile.close();
}


void additional_instructions (){
    char response;
    cout << "Do you have additional instructions? If so, enter 'Y'. " << endl;
    cin >> response;

    if(response =='Y' || response =='y'){
        display_menu();
        cin >> user_added_opcode;        //***CONVERT USER INPUT TO AL CAPS
        valid_entry = translation.find(user_added_opcode);
        {
            if(valid_entry == translation.end()){
                cout << "Error - Action not known." << endl;
            }
            else{
                RAM.push_back(valid_entry->second);
            }
        }
        cout << "Step 2: Enter the operand: " <<endl;
        cin >> new_operand;
        RAM.push_back(new_operand);

        for(int i=0; i<RAM.size(); i++){        // FETCH CYCLE
            opcode = RAM[PC];
            execute_instructions(opcode);
            cout << "New value: " << ACC << endl;
        }
    }
    else{
        cout << "Exiting program." << endl;
        //***GO BACK TO MAIN MENU;
    }
}

void display_menu(){
    cout << "Step 1 - Select an operation to perform: " <<endl;
    cout << "Enter CLR to clear last calculated value, OR" << endl;
    cout << "Enter LOAD to load an operand to the accumulator, OR" << endl;
    cout << "Enter ADD to add operand to last calculated value, OR" << endl;
    cout << "Enter SUB to subtract operand from last calculated value, OR" << endl;
    cout << "Enter MUL to multiply last calculated value by an operand, OR" << endl;
    cout << "Enter DIV to divide last calculated value by an operand, OR" << endl;
    cout << "Enter OUT to display last calculated value, OR" << endl;
}
