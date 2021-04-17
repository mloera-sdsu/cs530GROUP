#include "a2.h"

// default constructor
SICXE_Parser::SICXE_Parser() {
    fp = NULL;
    term = "";
    check = false;
}

// reads in the list, might need to pass a string mode parameter to specify type of input file
void SICXE_Parser::Read() {
    char c = fgetc(fp);
    if(isspace(c)){
        if(term.length()>0) {
            cout<<term<<endl;
            check = true;
        }
        term = "";
    }
    else{
        term = term + c;
    }
}

// writes to a file, might need to pass a string mode parameter to specify type of output file
void SICXE_Parser::Write() {

}
