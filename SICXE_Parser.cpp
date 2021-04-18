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
    string filename;
    string record;

    RemoveFileExtension(filename);
    for (int i = 0; i < sections.size();i++) { // for every section
        record += BuildHeaderRecord(i);
        record += BuildExtDef(i);
        record += BuildExtRef(i);
        record += BuildTextRecord(i);
        record += BuildModRecord(i);
    }

}

void SICXE_Parser::RemoveFileExtension(string &filename) {
    size_t dotIdx = filename.find_last_of(".");
    filename = filename.substr(0, dotIdx);
}

string SICXE_Parser::BuildHeaderRecord(int idx) {
    string headerRecStr;
    stringstream stream;
    SICXE_Source section = sections.at(idx);

    headerRecStr += HEADEROBJ;
    headerRecStr += section.name + SPACE;
    stream << setfill('0') << setw(ADDR_DIGIT_PLACES) << hex << section.start << section.end << endl;
    headerRecStr += stream.str();
    return headerRecStr;
}

string SICXE_Parser::BuildExtDef(int idx) {
    string extDefRecStr, tmp;
    stringstream stream;
    SICXE_Source section = sections.at(idx);
    bool found;

    extDefRecStr += EXTDEFOBJ;
    for (int i = 0;i < section.extdef.size();i++) { // every extdef entry of source
        tmp = section.extdef.at(i);
        extDefRecStr += tmp + SPACE;
        // search for extdef in instructions
        found = false;
        for (int j = 0;j < section.instructions.size() && !found;i++) { // every instruction of source
            if (section.instructions.at(j).label == tmp) {
                stream << setfill('0') << setw(ADDR_DIGIT_PLACES) << hex << section.instructions.at(j).addr;
                extDefRecStr += stream.str();
                stream.clear();
                found = true;
            }
        }
        if(!found) { // definition never found in section
            errno = ENXIO;
            fprintf(stderr, "EXTDEF %s defined but not loaded", tmp);
            perror("");
        }
    }
}

string SICXE_Parser::BuildExtRef(int idx) {

}
