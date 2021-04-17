#include "a2.h"

// default constructor
SICXE_Source::SICXE_Source() {
    start = end = NULL;
}

void SICXE_Source::Term(string term){
    bool address = true;
    for(int i=0; i<term.length(); ++i){
        if(!isdigit(term[i])){  //Check if all are digits meaning address
            address = false;
        }
        else{
            //Do something with address
        }
    }

}
