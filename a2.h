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
#include <algorithm>
#include <math.h>

#define FIRST_ARG 1
#define ADDR_COL 1
#define SECOND_COL 2
#define THIRD_COL 3
#define OBJCODE_COL 5
#define START 0
#define EXTDEF 3
#define EXTREF 4
#define DEFAULT_MOD_SIZE 5
#define WORD_MOD_SIZE 6
#define HEADEROBJ 'H'
#define EXTDEFOBJ 'D'
#define EXTREFOBJ 'R'
#define TEXTOBJ 'T'
#define MODOBJ 'M'
#define ENDOBJ 'E'
#define M_WORD "WORD"
#define OBJECT_EXTENSION ".obj"
#define TEXTREC_BYTE_LIMIT 16
#define SPACE ' '
#define PLUS '+'
#define MINUS '-'
#define COMMA ','
#define HASHTAG '#'
#define FAIL_FIND -1
#define ADDR_DIGIT_PLACES 6
#define EXT_ADDR_DIGIT_PLACES 8
#define FORMAT_3_BITS 12
#define FORMAT_4_BITS 20

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
    void MemCheck();
    void WriteObjFile();
    void WriteSymTabFile();
private:
    uint32_t stringToHex(string token);
    string BuildHeaderRecord(int idx);
    string BuildExtDef(int idx);
    string BuildExtRef(int idx);
    string BuildTextRecord(int idx);
    string BuildModRecord(int idx);
    string BuildEndRecord(int idx);
    string RemoveFileExtension(string filename);
    int HasExtRef(string token, int sectionIdx);
    char LeadingPlusOrMinusCheck(string token);
    string SymTabDefs(SICXE_Source section, uint32_t start);
    string SymTabSections(string s_name, uint32_t s_start, uint32_t lengths);
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
    set<string> mnemonics, directives;

    SICXE_Dictionary();
};
