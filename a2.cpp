#include "a2.h"

int main(int *argc, char *argv[]) {
    SICXE_Parser parser;

    Get_Set_Parser(*argc, argv, parser);

    return EXIT_SUCCESS;
}

// read in commands and initialize SICXE_Parser
void Get_Set_Parser(int &argc, char *argv[], SICXE_Parser &parser) {
    int argument = 1;
    parser.fp = fopen(argv[argument], "r");
    if (parser.fp == NULL){
        cout<<"Cannot open file"<<endl;
        return;
    }
    while(!feof(parser.fp)){
        parser.Read();
        
    }
}
