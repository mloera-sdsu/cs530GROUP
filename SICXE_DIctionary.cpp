#include "a2.h"

// default constructor initializes the dict
SICXE_Dictionary::SICXE_Dictionary() {
    mnemonics = { "ADD", "+ADD", "+ADDF", "ADDF", "ADDR", "+AND", "AND", "CLEAR", "+COMP"
    , "COMP", "+COMPF", "COMPF", "COMPR", "+DIV", "DIV", "+DIVF" "DIVF", "DIVR", "FIX", "FLOAT"
    , "HIO", "+J", "J", "+JEQ", "JEQ", "+JGT", "JGT", "+JLT", "JLT", "+JSUB", "JSUB", "+LDA"
    , "LDA", "+LDB", "LDB", "+LDCH", "LDCH", "+LDF", "LDF", "+LDL", "LDL", "+LDS", "LDS", "+LDT"
    , "LDT", "+LDX", "LDX", "+LPS", "LPS", "+MUL", "MUL", "+MULF", "MULF", "MULR", "NORM", "+OR"
    , "OR", "+RD", "RD", "RMO", "+RSUB", "RSUB", "SHIFTL", "SHIFTR", "SIO", "+SSK", "SSK", "+STA"
    , "STA", "+STB", "STB", "+STCH", "STCH", "+STF", "STF", "+STI", "STI", "+STL", "STL", "+STS"
    , "STS", "+STSW", "STSW", "+STT", "STT", "+STX", "STX", "+SUB", "SUB", "+SUBF", "SUBF", "SUBR"
    ,"START", "END", "ORG", "LTORG", "BASE", "NOBASE", "CSECT", "USE", "EQU", "RESB", "RESW", "EXTDEF"
    , "EXTREF", "SVC", "+TD", "TD", "TIO", "+TIX", "TIX", "TIXR", "+WD", "WD", }; // <---add in here
}
