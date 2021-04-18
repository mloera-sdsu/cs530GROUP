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
    bool comma;
    string line, token, previousToken;
    SICXE_Source curSection;
    SICXE_Instruction curInstruction;
    curInst
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
                int lineCount = curSection.instructions.size();
                curInstruction = SICXE_Instruction();
                for (int j = 0;line.at(j);j++) { // char by char
                    c = line.at(j);
                    if(c == ',')
                        comma = true;
                    if (lineCount==0){ // first line
                        if(isspace(c) && token.size() != 0){// word defining condition for 1st line
                            ++wordcount;
                            if(wordcount == ADDR_COL)
                                curInstruction.addr = stoul(token);
                            if(wordcount == 2)
                                curSection.name = token;
                            if(wordcount == 4)
                                curSection.start = stoul(token);
                        }
                        else if (!isspace(c)) {
                        token.push_back(c);
                        }
                    }
                    else if (lineCount == 1){ // ext def line
                        if(comma && token.size()!=0){
                            curSection.extdef.push_back(token);
                        }
                        if(isspace(c) && token.size() != 0){
                            ++wordcount;
                            if(token.compare("EXTDEF"))
                                wordcount = 3;
                        }
                    }
                    else if (lineCount == 2){ // ext ref line
                        
                    }
                    else{ // in general
                    //0035 LDX
                        if (isspace(c) && token.size() != 0) { // word defining condition for in general
                            //cout << token << endl;
                            check = true;
                            //curInstruction.addr = stoul(token);
                            //wordcount = CheckToken(token, wordcount, curSection.extdef);
                            previousToken = token;
                            token.erase(); // token = ""
                        }
                        else if (!isspace(c)) {
                            token.push_back(c); //0035    LDX
                        }
                    }
                }
                curSection.instructions.emplace_back(curInstruction);
            }
            sectionsIdx++;
        }
    }
}

int SICXE_Parser::CheckToken(string token, int column, vector<string> defs){
    if(token.compare("START") == 0){
        column = START;
    }
    else if(token.compare("EXTDEF") == 0){
        column = EXTDEF;
    }
    else if(token.compare("EXTREF") == 0){
        column = EXTREF;
    }
    else if(column == 0 && token.compare("END") != 0){
        column = ADDR_COL;
    }
    return column;
}
// writes to a file, might need to pass a string mode parameter to specify type of output file
void SICXE_Parser::Write() {

}

