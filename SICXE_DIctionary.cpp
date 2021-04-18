#include "a2.h"

// default constructor initializes the dict
SICXE_Dictionary::SICXE_Dictionary() {
    mnemonics = {"ADD", "ADDF", "ADDR", "AND", "CLEAR", "COMP", "COMPF", "COMPR", "DIV", "DIVF", "DIVR", "FIX", "FLOAT"
    , "HIO", "J", "JEQ", "JGT", "JLT", "JSUB", "LDA", "LDB", "LDCH", "LDF", "LDL", "LDS", "LDT", "LDX", "LPS", "MUL", "MULF", "MULR"
    , "NORM", "OR", "RD", "RMO", "RSUB", "SHIFTL", "SHIFTR", "SIO", "SSK", "STA", "STB", "STCH", "STF", "STI", "STL", "STS", "STSW"
    , "STT", "STX", "SUB", "SUBF", "SUBR", "SVC", "TD", "TIO", "TIX", "TIXR", "WD"}; // <---add in here
}