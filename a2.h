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
#include <algorithm>

#define FIRST_ARG 1
#define ADDR_COL 0
#define HEADEROBJ 'H'
#define EXTDEFOBJ 'D'
#define SPACE ' '
#define ADDR_DIGIT_PLACES 6

using namespace std;

class SICXE_Instruction;
class SICXE_Source;
class SICXE_Parser;

// does all the io stuff and uses SICXE classes to store data
class SICXE_Parser {
public:
    ifstream infile;
    ofstream outfile;
    vector<string> paths;
    vector<SICXE_Source> sections;

    SICXE_Parser();
    SICXE_Parser(int &argc, char *argv[]);
    void Read();
    void Write();
private:
    string BuildHeaderRecord(int idx);
    string BuildExtDef(int idx);
    string BuildExtRef(int idx);
    string BuildTextRecord(int idx);
    string BuildModRecord(int idx);
    void RemoveFileExtension(string &filename);
};

// source file contents consisting of an array of SICXE_Instruction objects
class SICXE_Source {
public:
    string name, filename;
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
