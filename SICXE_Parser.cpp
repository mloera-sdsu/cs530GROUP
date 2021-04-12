#include "a2.h"

// default constructor
SICXE_Parser::SICXE_Parser() {
    fp = NULL;
}

// reads in the list, might need to pass a string mode parameter to specify type of input file
void SICXE_Parser::Read() {
    char c = fgetc(fp);
    cout<<c;
    if(isspace(c)){
        
    }


}

// writes to a file, might need to pass a string mode parameter to specify type of output file
void SICXE_Parser::Write() {

}
