#include <iostream>
#include <string>
#include <stdio.h>
#include <errno.h>
#include <vector>
#include <iomanip>
#include <math.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>

#define FIRST_ARG 1
#define ADDR_COL 1
#define SECOND_COL 2
#define START 0 
#define EXTDEF 3
#define EXTREF 4

using namespace std;

class SICXE_Instruction;
class SICXE_Source;
class SICXE_Parser;

void Get_Set_Parser(int &argc, char *argv[], SICXE_Parser &parser);

// does all the io stuff and uses SICXE classes to store data
class SICXE_Parser {
public:
    ifstream infile;\
    vector<string> paths;
    vector<SICXE_Source> sections;

    SICXE_Parser();
    SICXE_Parser(int &argc, char *argv[]);
    void Read();
    int CheckToken(string token, int column, vector<string> defs);
    void Write();
};

// source file contents consisting of an array of SICXE_Instruction objects
class SICXE_Source {
public:
    string name;
    vector<string> extdef, extref;
    vector<SICXE_Instruction> instructions;
    uint32_t start, end;

    SICXE_Source();
    void Term(string term);


};

// one line of sic/xe with address label mnemonic etc..
// each field could be expanded to be a class or enum later if it makes sense
class SICXE_Instruction {
public:
    uint32_t addr, objcode;
    string label, mnemonic;
    vector<string>args;
    bool hasEXTREF;

    SICXE_Instruction();
};
