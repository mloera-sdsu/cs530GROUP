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
#include <set>

#define FIRST_ARG 1
#define ADDR_COL 1
#define SECOND_COL 2
#define START 0
#define EXTDEF 3
#define EXTREF 4
#define HEADEROBJ 'H'
#define EXTDEFOBJ 'D'
#define EXTREFOBJ 'R'
#define TEXTOBJ 'T'
#define TEXTREC_BYTE_LIMIT 16
#define SPACE ' '
#define ADDR_DIGIT_PLACES 6
#define EXT_ADDR_DIGIT_PLACES 8

using namespace std;

class SICXE_Dictionary;
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
    int CheckToken(string token, int column, vector<string> defs);
    void WriteObjFile();
    void WriteSymTabFile();
private:
    uint32_t stringToHex(string token);
    string BuildHeaderRecord(int idx);
    string BuildExtDef(int idx);
    string BuildExtRef(int idx);
    string BuildTextRecord(int idx);
    string BuildModRecord(int idx);
    string RemoveFileExtension(string filename);
};

// source file contents consisting of an array of SICXE_Instruction objects
class SICXE_Source {
public:
    string name, filename;
    vector<string> extdef, extref;
    vector<SICXE_Instruction> instructions;
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
    bool hasEXTREF;

    SICXE_Instruction();
};

class SICXE_Dictionary {
public:
    set<string> mnemonics = {};

    SICXE_Dictionary();
};
