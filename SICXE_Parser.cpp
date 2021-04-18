#include "a2.h"

// custom constructor
SICXE_Parser::SICXE_Parser(int &argc, char *argv[]) {
    for (int i = FIRST_ARG;i < argc;i++)
        paths.push_back(argv[i]);
}

// reads in the list, might need to pass a string mode parameter to specify type of input file
void SICXE_Parser::Read() {
    char c;
    int wordcount, sectionsIdx = 0;
    bool check;
    string line, token;
    SICXE_Source curSection;
    SICXE_Instruction curInstruction;

    // for every path
    for (int i = 0;i < paths.size();i++) {
        infile.open(paths.at(i));
        if (!infile.good()) {
            errno = ENOENT;
            perror("File path error");
        }
        else { // file by file
            sections.emplace_back(SICXE_Source());
            curSection = sections.back();
            while (getline(infile, line)) { // line by line
                wordcount = 0;
                curSection.instructions.emplace_back(SICXE_Instruction());
                curInstruction = curSection.instructions.back();

                for (int j = 0;line.at(j);j++) { // char by char
                    c = line.at(j);

                    if (isspace(c) && token.size() != 0) {
                        //cout << token << endl;
                        check = true;
                        switch (wordcount) {
                        case ADDR_COL:
                            curInstruction.addr = stoul(token);
                            break;
                        default:
                            break;
                        }
                        token.erase();
                    }
                    else if (!isspace(c)) {
                        token.push_back(c);
                    }
                }
            }
            sectionsIdx++;
        }
    }
}


// writes to a file, might need to pass a string mode parameter to specify type of output file
void SICXE_Parser::Write() {

}

