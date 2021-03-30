#include <iostream>
#include <string>
#include <stdio.h>
#include <errno.h>
#include <vector>
#include <iomanip>
#include <math.h>
#include <stdlib.h>

using namespace std;

class SICXE_Instruction;
class SICXE_Source;
class SICXE_Parser;

void Get_Set_Parser(int &argc, char *argv[], SICXE_Parser &parser);

// does all the io stuff and uses SICXE classes to store data
class SICXE_Parser {
public:
    FILE *fp;
    vector<string> paths;
    vector<SICXE_Source *> sections;

    SICXE_Parser();

    void Read();
    void Write();
};

// source file contents consisting of an array of SICXE_Instruction objects
class SICXE_Source {
public:
    string name;
    vector<string> extdef, extref;
    vector<SICXE_Instruction *> instructions;
    uint32_t start, end;

    SICXE_Source();
};

// one line of sic/xe with address label mnemonic etc..
// each field could be expanded to be a class or enum later if it makes sense
class SICXE_Instruction {
public:
    uint32_t addr, objcode;
    string label, mnemonic;
    vector<string>args;

    SICXE_Instruction();
};