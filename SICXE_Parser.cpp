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
    string line, token, previousToken;
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
                        wordcount = CheckToken(token, wordcount);
                        switch (wordcount) {
                            case ADDR_COL:
                                curInstruction.addr = stoi(token);
                                break;
                            case START:
                                curSection.name = previousToken;
                                token.erase();
                                while(isspace(c)){
                                    j++;
                                    c = line.at(j);
                                }
                                while(!isspace(c)){
                                    token.push_back(c);
                                    ++j;
                                    c = line.at(j);
                                }
                                curSection.start = stoi(token);
                                break;
                            case EXTDEF:
                                token.erase();
                                while(isspace(c)){
                                    ++j;
                                    c = line.at(j);
                                }
                                while(!isspace(c)){
                                    token.push_back(c);
                                    ++j;
                                    c = line.at(j);
                                    if(c == ','){
                                        curSection.extdef.push_back(token);
                                        token.erase();
                                        ++j;
                                        c = line.at(j);
                                    }
                                }
                                break;
                            case EXTREF:
                                token.erase();
                                while(isspace(c)){
                                    ++j;
                                    c = line.at(j);
                                }
                                while(!isspace(c)){
                                    token.push_back(c);
                                    ++j;
                                    c = line.at(j);
                                    if(c == ','){
                                        curSection.extref.push_back(token);
                                        token.erase();
                                        ++j;
                                        c = line.at(j);
                                    }
                                }
                            default:
                                break;

                        }
                        previousToken = token;
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

