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
    bool extdef_F = false;
    bool extref_F = false;
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
            curSection.filename = RemoveFileExtension(paths.at(i));
            while (getline(infile, line)) { // line by line
                wordcount = 0;
                int lineCount = curSection.instructions.size();
                curInstruction = SICXE_Instruction();
                for (int j = 0;line.at(j);j++) { // char by char
                    c = line.at(j);
                    if (c == ',')
                        comma = true;

                    if (lineCount == 0) { // first line
                        if (isspace(c) && token.size() != 0) {// word defining condition for 1st line
                            ++wordcount;
                            if (wordcount == ADDR_COL)
                                curInstruction.addr = stringToHex(token);
                            if (wordcount == 2)
                                curSection.name = token;
                            if (wordcount == 4)
                                curSection.start = stringToHex(token);
                            token.erase();
                        }
                        else if (!isspace(c)) {
                            token.push_back(c);
                        }
                    }
                    else if (lineCount == 1) { // ext def line
                        if (comma && token.size() != 0 && extdef_F) {
                            curSection.extdef.push_back(token);
                            comma = false;
                            token.erase();
                        }
                        if (isspace(c) && token.size() != 0) {
                            ++wordcount;
                            if (token.compare("EXTDEF")) //CHECK IF THIS IS NEEDED
                                extdef_F = true;
                            token.erase();
                        }
                        else if (!isspace(c)) {
                            token.push_back(c);
                        }
                    }
                    else if (lineCount == 2) { // ext ref line
                        if (comma && token.size() != 0 && extref_F) {
                            curSection.extref.push_back(token);
                            comma = false;
                            token.erase();
                        }
                        if (isspace(c) && token.size() != 0) {
                            ++wordcount;
                            if (token.compare("EXTREF")) //CHECK IF THIS IS NEEDED
                                extref_F = true;
                            token.erase();
                        }
                        else if (!isspace(c)) {
                            token.push_back(c);
                        }
                    }
                    else { // in general

                        if ((isspace(c) && token.size() != 0) /*|| (comma && token.size()!=0)*/) { // word defining condition for in general
                            if (curInstruction.addr == NULL) {
                                curInstruction.addr = stringToHex(token);
                                token.erase();
                            }
                            else if (curInstruction.label.compare("NULL") && curInstruction.mnemonic.compare("NULL") /*&& not in dict*/) {
                                curInstruction.label = token;
                                token.erase();
                            }
                            else if (curInstruction.mnemonic.compare("NULL")) {
                                curInstruction.mnemonic = token;
                                token.erase();
                            }
                            else if (curInstruction.args.empty()) { //Check if vector string is empty
                                if (token.find('+') != string::npos || token.find('-') != string::npos) { //Check if token has + or -

                                    //Handle parenthesis first
                                    //Change arithmtic if '-' outside parenthesis
                                    //remove parenthesis
                                    bool parenthesis = false;
                                    if (token.find('(') != string::npos || token.find(')') != string::npos) {
                                        for (int q = 0; q < token.size() - 1; q++) {
                                            if (token[q] == '(' && token[q - 1] == '-') {
                                                parenthesis == true;
                                            }
                                            else if (token[q] == ')') {
                                                parenthesis == false;
                                            }

                                            if ((token[q] == '-') && parenthesis == true) {
                                                token[q] == '+';
                                            }
                                            else if ((token[q] == '+') && parenthesis == true) {
                                                token[q] == '-';
                                            }
                                        }
                                        token.erase(remove(token.begin(), token.end(), '('), token.end());
                                        token.erase(remove(token.begin(), token.end(), ')'), token.end());
                                    }

                                    //Seperate args based on +/- and add them to vector
                                    string tempPlusMinus;

                                    for (int k = 0; k < token.size() - 1; k++) {
                                        if (token[k] == '+' || token[k] == '-') {
                                            curInstruction.args.push_back(tempPlusMinus);
                                            tempPlusMinus = "";
                                            tempPlusMinus += token[k];
                                        }
                                        else {
                                            tempPlusMinus += token[k];
                                        }
                                    }
                                    curInstruction.args.push_back(tempPlusMinus);
                                    token.erase();
                                }
                                else if (token.find(',') != std::string::npos) { // check if token has comma
                                    string tempComma;
                                    for (int m = 0; m < token.size() - 1; m++) {
                                        if (token[m] != ',') {
                                            tempComma += token[m];
                                        }
                                        else if (token[m] == ',') {
                                            curInstruction.args.push_back(tempComma);
                                            tempComma = "";
                                        }
                                    }
                                    curInstruction.args.push_back(tempComma);
                                }
                                else {
                                    curInstruction.args.push_back(token);
                                }
                                token.erase();
                            }
                            else if (curInstruction.objcode == NULL && j == line.length() - 1) {
                                curInstruction.objcode = stringToHex(token);
                                token.erase();
                            }

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
uint32_t SICXE_Parser::stringToHex(string token) {
    stringstream ss;
    uint32_t token_Hex;
    ss << hex << token;
    ss >> token_Hex;
    return token_Hex;
}
int SICXE_Parser::CheckToken(string token, int column, vector<string> defs) {
    if (token.compare("START") == 0) {
        column = START;
    }
    else if (token.compare("EXTDEF") == 0) {
        column = EXTDEF;
    }
    else if (token.compare("EXTREF") == 0) {
        column = EXTREF;
    }
    else if (column == 0 && token.compare("END") != 0) {
        column = ADDR_COL;
    }
    return column;
}
// writes to a file, might need to pass a string mode parameter to specify type of output file
void SICXE_Parser::Write() {
    string filename;
    string record;

    for (int i = 0; i < sections.size();i++) { // for every section
        outfile.open(sections.at(i).filename, fstream::out);
        record += BuildHeaderRecord(i);
        record += BuildExtDef(i);
        record += BuildExtRef(i);
        record += BuildTextRecord(i);
        record += BuildModRecord(i);
        outfile.close();
    }
}

// pass argv raw arg for filepath and return the filename with no extention
string SICXE_Parser::RemoveFileExtension(string filename) {
    size_t start, end = 0;
    start = filename.find_last_of("/");
    if (start == 0) {
        start = filename.find_last_of("\\");
    }
    end = filename.find_last_of(".");
    filename = filename.substr(start, end);
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
            if (section.instructions.at(j).label == tmp && section.instructions.at(j).mnemonic == "EQU") {
                stream << setfill('0') << setw(ADDR_DIGIT_PLACES) << hex << section.instructions.at(j).addr;
                extDefRecStr += stream.str();
                stream.clear();
                found = true;
            }
        }
        if (!found) { // definition never found in section
            errno = ENXIO;
            fprintf(stderr, "EXTDEF %s defined but not loaded", tmp);
            perror("");
        }
    }
    return extDefRecStr + "\n";
}

string SICXE_Parser::BuildExtRef(int idx) {
    string extRefRecStr;
    SICXE_Source section = sections.at(idx);

    extRefRecStr += EXTREFOBJ;
    for (int i = 0;i < section.extref.size();i++)
        extRefRecStr += section.extref.at(i) + SPACE;
    return extRefRecStr + "\n";
}
// every 16 bytes (2 hex digits per byte)
string SICXE_Parser::BuildTextRecord(int idx) {
    string textRecStr;
    stringstream stream;
    SICXE_Source section = sections.at(idx);
    size_t idxForLength;
    int digitPlaces, bytecount;
    uint32_t addrcount;
    bool isOverLimit = true;

    addrcount = section.start;
    bytecount = 0;
    for (int i = 0;i < section.instructions.size();i++) {
        if (isOverLimit) {// starts at overlimit to set new TextRecord line
            addrcount += bytecount;
            bytecount = 0;
            isOverLimit = false;
            textRecStr += TEXTOBJ;
            stream << setfill('0') << setw(ADDR_DIGIT_PLACES) << hex << addrcount;
            textRecStr += stream.str();
            stream.clear();
            idxForLength = textRecStr.size();
        }
        // when in middle of parsing objcodes within limit of 16 bytes
        digitPlaces = ADDR_DIGIT_PLACES;
        if (section.instructions.at(i).mnemonic.find('+') != string::npos) // extended addressing case
            digitPlaces = EXT_ADDR_DIGIT_PLACES;

        if (bytecount + digitPlaces / 2 > TEXTREC_BYTE_LIMIT || i == section.instructions.size() - 1) {// check if surpassed byte limit or last instruction
            stream << hex << bytecount;
            textRecStr.insert(idxForLength, stream.str());
            textRecStr += "\n";
            stream.clear();
            isOverLimit = true;

            if (i != section.instructions.size() - 1)
                i--;
        }
        else { // within byte limit, add objcode
            bytecount += digitPlaces / 2;
            stream << setfill('0') << setw(digitPlaces) << hex << section.instructions.at(i).objcode;
            textRecStr += stream.str();
            stream.clear();
        }
    }
    return textRecStr;
}