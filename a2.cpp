#include "a2.h"

int main(int argc, char *argv[]) {
    SICXE_Parser parser(argc, argv);
    parser.Read();
    parser.MemCheck();
    parser.WriteObjFile();
    parser.WriteSymTabFile();
    return EXIT_SUCCESS;
}
