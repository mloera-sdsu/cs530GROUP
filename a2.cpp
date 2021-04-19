#include "a2.h"

int main(int argc, char *argv[]) {
    SICXE_Parser parser = SICXE_Parser(argc, argv); 
    parser.Read();
    parser.WriteSymTabFile();
    parser.WriteObjFile();
    return EXIT_SUCCESS;
}
