#include "a2.h"

int main(int argc, char *argv[]) {
    SICXE_Parser parser;
    parser.Read();
    parser.WriteSymTabFile();
    return EXIT_SUCCESS;
}
