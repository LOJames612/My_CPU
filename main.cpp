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
#include <locale>
#include <stdlib.h>
using namespace std;

map <string, int> translation;                  // Map relating each mnemonic to a number
map <string, int> ::iterator valid_entry;       // Map iterator
string filename_prompt();
void create_translation();
void execute_instructions(const int);
void process_file();
void additional_instructions ();
void display_menu();
int main_menu();
void about();
void display_file();
void execute_main_menu(int);

const int ABOUT = 1;
const int DISPLAY_FILE = 2;
const int EXECUTE_FILE = 3;
const int QUIT = 4;

int ACC, PC=0, opcode, operand, main_select, result, new_operand;    // Variables to hold values for accumulator, program counter
                                                                    // opcode, operand, result of string converted to int
vector<int> RAM;                                                    // Vector to hold opcodes and operands
string filename, entry, user_added_opcode;                          // Variables to hold filename and instructions
char user_entry;                                                    // (opcodes and operands) to be interpreted by translator

int main()
{
    create_translation();                       // Translate mnemonics to numbers for execution

    main_select = main_menu();
    execute_main_menu(main_select);

    return 0;
}


//Prompt for filename input
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
    translation ["STORE"] = 9;
    translation ["HALT"] = 0xF4;
}


//Function to execute instructions based on mnemonics
void execute_instructions(const int opcode){
    switch(opcode){
        case 0:                         // Clear ACC to zero
            ACC = 0;
            PC++;
            break;
        case 1:                         // DEFINE THIS!
            operand = RAM[PC++];
            ACC = RAM[PC];
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
        case 8:                         // Branch Not Zero
            operand = RAM[++PC];
            if ( ACC != 0 )
                PC = operand;
            else
                PC++;
            break;
        case 9:                                 // Store ACC to a memory location
            operand = RAM[++PC];
            RAM[PC] = ACC;
        case 0xF4:                              // Halt program
            cout << "Program halted" << endl;
            break;
    }
}


void process_file(){

    filename = filename_prompt();               // Prompt for name of file
    ifstream myfile(filename.c_str());          // Stream in contents file

    if (myfile.is_open()){
        while (myfile >> entry && !myfile.eof()){   // Reads in each element in uploaded file
            valid_entry = translation.find(entry);  // Translates elements into CPU language (defined in map)
            if (valid_entry == translation.end()){    //If match is found in map, push entry to RAM
                istringstream(entry) >> result;
                RAM.push_back(result);
                }
            else
                RAM.push_back(valid_entry->second);        //If match is found in map, push translated instruction to RAM
        }

        for(int i=0; i<RAM.size(); i++){                    // Fetch cycle
            opcode = RAM[PC];
            execute_instructions(opcode);
        }
    }
    else {
            cout <<endl;
            cout << "Error in opening your file, try again." << endl;
            main_select = main_menu();
            execute_main_menu(main_select);
    }

    myfile.close();
}


void additional_instructions (){        //FUTURE WORK: Loop to allow more than one additional instruction
    char response;
    cout << "Do you have additional instructions? If so, enter 'Y'. " << endl;
    cout << "If not, press any other key and you will be returned to the main menu." <<endl;
    cin >> response;

    if(response =='Y' || response =='y'){
        display_menu();                                     //Prompt user for additional opcode
        cin >> user_added_opcode;
        valid_entry = translation.find(user_added_opcode);  // Match user entry against translation map
        {
            if(valid_entry == translation.end()){
                cout <<endl;
                cout << "Error - Action unknown." << endl;  //In case of unknown opcode
                additional_instructions();
            }
            else{
                RAM.push_back(valid_entry->second);           // If match is found in map, push translated instruction to RAM
            }
        }
        cout << endl;
        cout << "Step 2: Enter the operand: " <<endl;           //Prompt for user's operand
        cin >> new_operand;
        RAM.push_back(new_operand);

        for(int i=0; i<RAM.size(); i++){                        // Fetch cycle
            opcode = RAM[PC];
            execute_instructions(opcode);

        }
        cout <<endl;
        cout << "New value: " << ACC << endl;
        cout << endl;
        main_select = main_menu();
        execute_main_menu(main_select);
    }
    else{
        cout <<endl;
        cout << "Returning to main menu..." << endl;
        cout << endl;
        main_select = main_menu();
        execute_main_menu(main_select);
    }
}


//Menu for additional instruction options
void display_menu(){
    cout <<endl;
    cout << "Step 1 - Select an operation to perform: " <<endl;
    cout << "Enter CLR to clear last calculated value, OR" << endl;
    cout << "Enter LOAD to load an operand to the accumulator, OR" << endl;
    cout << "Enter ADD to add operand to last calculated value, OR" << endl;
    cout << "Enter SUB to subtract operand from last calculated value, OR" << endl;
    cout << "Enter MUL to multiply last calculated value by an operand, OR" << endl;
    cout << "Enter DIV to divide last calculated value by an operand, OR" << endl;
    cout << "Enter OUT to display last calculated value."<< endl;
}


//Main menu
int main_menu(){
    int selection;

    cout <<endl;
    cout << "Welcome to Olivia's CPU! Please select an option:" <<endl;
    cout << "1..... About" <<endl;
    cout << "2..... Display file contents" <<endl;
    cout << "3..... Execute file" <<endl;
    cout << "4..... Quit" <<endl;
    cin >> selection;
    return selection;
}


void about(){
    cout <<endl;
    cout << "CPU created by Olivia James." << endl;
    cout << "(c) Copyright 2015. All Rights Reserved." << endl;
    cout << "There are 10 types of people in this world: those who know binary and those who don't." <<endl;
    cout << "#girlsbuiltthis" <<endl;
}


void display_file(){

    filename = filename_prompt();               // Prompt for name of file
    ifstream myfile(filename.c_str());          // Stream in contents file

    if (myfile.is_open()){
        while (myfile >> entry && !myfile.eof()){   // Reads in each element in uploaded file
            cout << entry <<endl;
        }
    }
        else
            cout << "File could not be opened." <<endl;
}


void execute_main_menu(int main_select){

    switch (main_select){
        case 1:
            cout << endl;
            about();
            cout <<endl;
            cout << "Enter 'M' to return to the main menu: " <<endl;
            cin >> user_entry;
            if (user_entry == 'M' || user_entry == 'm'){
                    system("CLS");                      //Clear screen
                    main_select = main_menu();          //Back to main menu
                    execute_main_menu(main_select);
                }
            break;
        case 2:
            cout << endl;
            display_file();
            cout << endl;
            cout << "Enter 'M' to return to the main menu: " <<endl;
            cin >> user_entry;
                if (user_entry == 'M' || user_entry == 'm'){
                    system("CLS");
                    main_select = main_menu();
                    execute_main_menu(main_select);
                }
            break;
        case 3:
            cout << endl;
            process_file();                 // Process and execute file
            cout <<endl;
            additional_instructions ();     // Ask user for additional instructions
            break;
        case 4:
            exit(EXIT_SUCCESS);
        default:
            cout << endl;
            cout << "Sorry, that is not an option. Please select from one of the options below." <<endl;
            //FUTURE WORK: Loop to prompt for filename re-entry when previous entries fail.
            main_select = main_menu();
            execute_main_menu(main_select);
    }
}
