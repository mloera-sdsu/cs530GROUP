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
    bool end = false;
    bool inDictionary;
    string line, token, previousToken;
    SICXE_Source curSection;
    SICXE_Instruction curInstruction;
    SICXE_Dictionary mnemonicsDictionary;
    // for every path
    for (int i = 0;i < paths.size();i++) {
        infile.open(paths.at(i));
        if (!infile.good()) {
            errno = ENOENT;
            perror("File path error");
        }
        else { // file by file
            curSection = SICXE_Source();
            curSection.filename = RemoveFileExtension(paths.at(i));
            while (getline(infile, line)) { // line by line
                wordcount = 0;
                int lineCount = curSection.instructions.size();
                curInstruction = SICXE_Instruction();
                for (int j = 0;j < line.size();j++) { // char by char

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
                            token.clear();
                        }
                        else if (!isspace(c)) {
                            token.push_back(c);
                            if (j == line.size() - 1) {
                                wordcount++;
                                if (wordcount == 4)
                                    curSection.start = stringToHex(token);
                                token.clear();
                            }
                        }
                    }
                    else if (lineCount == 1) { // ext def line
                        if (comma && token.size() != 0 && extdef_F) {
                            curSection.extdef.push_back(token);
                            comma = false;
                            token.clear();
                        }
                        else if (isspace(c) && token.size() != 0) {
                            ++wordcount;
                            if (token == "EXTDEF") //CHECK IF THIS IS NEEDED
                                extdef_F = true;
                            if (wordcount == ADDR_COL)
                                curInstruction.addr = stringToHex(token);
                            token.clear();
                        }
                        else if (!isspace(c)) {
                            token.push_back(c);
                            if (j == line.size() - 1) {
                                if (token.size() != 0 && extdef_F) {
                                    curSection.extdef.push_back(token);
                                    token.clear();
                                }
                            }
                        }
                    }
                    else if (lineCount == 2) { // ext ref line
                        if (comma && token.size() != 0 && extref_F) {
                            curSection.extref.push_back(token);
                            comma = false;
                            token.clear();
                        }
                        if (isspace(c) && token.size() != 0) {
                            ++wordcount;
                            if (token == "EXTREF") //CHECK IF THIS IS NEEDED
                                extref_F = true;
                            token.clear();
                        }
                        else if (!isspace(c)) {
                            token.push_back(c);
                            if (j == line.size() - 1) {
                                if (token.size() != 0 && extref_F) {
                                    curSection.extref.push_back(token);
                                    token.clear();
                                }
                            }
                        }
                    }
                    else { // in general
                        if ((isspace(c) && token.size() != 0)) { // word defining condition for in general
                            wordcount++;

                            if (token == "END" || end) { //First checks for "END"
                                if (token == "END")
                                    curInstruction.mnemonic = token;
                                for (int i = 0; i < curSection.instructions.size();++i) {
                                    if (token.compare(curSection.instructions[i].label) == 0) { //Checks all labels to grab address
                                        curInstruction.addr = curSection.instructions[i].addr;
                                    }
                                }
                                token.clear();
                                end = true;
                            }
                            if (wordcount == ADDR_COL) {        //Handles adding addr token
                                curInstruction.addr = stringToHex(token);
                                token.clear();
                            }
                            else if (wordcount == SECOND_COL) { //handles adding label token
                                if (mnemonicsDictionary.mnemonics.count(token)) {  //check if token is in dictionary
                                    curInstruction.mnemonic = token;
                                    wordcount++;
                                }
                                else
                                    curInstruction.label = token;
                                token.clear();
                            }
                            else if (wordcount == THIRD_COL) { //Handles adding mnemonic token
                                curInstruction.mnemonic = token;
                                token.clear();
                            }
                            else if (curInstruction.args.empty()) { //Handles adding args (1st section if +/-)(2nd section if ',')(3rd if jusst token)
                                if (token.find('+') != string::npos || token.find('-') != string::npos) { //Check if token has + or -
                                    string tempPlusMinus; //Seperate args based on +/- and add them to vector

                                    //Handle parenthesis first
                                    //Change arithmtic if '-' outside parenthesis
                                    //remove parenthesis
                                    bool parenthesis = false;
                                    if (token.find('(') != string::npos || token.find(')') != string::npos) {
                                        for (int q = 0; q < token.size() - 1; q++) {
                                            if (token[q + 1] == '(' && token[q] == '-') {
                                                parenthesis = true;
                                                q += 2;
                                            }
                                            else if (token[q] == ')') {
                                                parenthesis = false;
                                            }

                                            if ((token[q] == '-') && parenthesis == true) {
                                                token[q] = '+';
                                            }
                                            else if ((token[q] == '+') && parenthesis == true) {
                                                token[q] = '-';
                                            }
                                        }
                                        token.erase(remove(token.begin(), token.end(), '('), token.end());
                                        token.erase(remove(token.begin(), token.end(), ')'), token.end());
                                    }
                                    for (int k = 0; k < token.size(); k++) {
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
                                    token.clear();
                                }
                                // Checks if token has comma and seperates and adds them to args
                                else if (token.find(',') != std::string::npos) {
                                    string tempComma;
                                    for (int m = 0; m < token.size(); m++) {
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
                                token.clear();
                            }
                            else if (curInstruction.objcode == NULL && j == line.length() - 1) { //Handles adding object code tokens
                                curInstruction.objcode = stringToHex(token);
                                token.clear();
                            }
                            previousToken = token;
                            token.clear(); // token = ""

                        }
                        else if (!isspace(c)) {
                            token.push_back(c); //0035    LDX
                            if (j == line.size() - 1) {
                                wordcount++;
                                if (curInstruction.args.empty())
                                    curInstruction.args.push_back(token);
                                else if (wordcount == OBJCODE_COL)
                                    curInstruction.objcode = stringToHex(token);
                                token.clear();
                            }
                        }
                    }
                }
                curSection.instructions.emplace_back(curInstruction);
            }
            //Find end address
            int sizeOfVector = curSection.instructions.size();
            if (curSection.instructions.at(sizeOfVector - 1).mnemonic == "END") { //Check if the last instruction is END
                curSection.end = curSection.instructions.at(sizeOfVector - 2).addr; //Get address of instruction before END
                curSection.end += 3;
            }
            else {
                curSection.end = curSection.instructions.at(sizeOfVector - 1).addr; //Last instruction address
                curSection.end += 3;
            }
            sections.emplace_back(curSection);
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
// writes to a file, might need to pass a string mode parameter to specify type of output file
void SICXE_Parser::WriteObjFile() {
    string filename;
    string record;

    for (int i = 0; i < sections.size();i++) { // for every section
        outfile.open(sections.at(i).filename + OBJECT_EXTENSION, fstream::out);
        record += BuildHeaderRecord(i);
        record += BuildExtDef(i);
        record += BuildExtRef(i);
        record += BuildTextRecord(i);
        record += BuildModRecord(i);
        outfile << record;
        outfile.close();
    }
}

void SICXE_Parser::WriteSymTabFile() {
    stringstream write;
    string record = "";
    string filename = "ESTAB.st";
    SICXE_Source source;
    string s_name;
    uint32_t s_start;
    uint32_t s_end;
    uint32_t length;

    outfile.open(filename, fstream::out);
    for (int i = 0; i < sections.size(); ++i) {
        source = sections.at(i);
        s_name = sections.at(i).name;
        if (i == 0) {
            s_start = sections.at(i).start; //000000
            s_end = sections.at(i).end; // 002F09
            length = s_end - s_start; // 002F09
            record += SymTabSections(s_name, s_start, s_end);
            record += SymTabDefs(source, length);
        }
        else {
            s_start = s_end; // Next section starting address is the end of the previous
            length = sections.at(i).end - sections.at(i).start;
            s_end = s_start + length;
            record += SymTabSections(s_name, s_start, s_end);
            record += SymTabDefs(source, length);
        }
    }
    outfile << record;
    outfile.close();
}
string SICXE_Parser::SymTabSections(string s_name, uint32_t s_start, uint32_t length) {
    stringstream write;
    string record;
    write << setfill(SPACE) << setw(8) << s_name;
    record += write.str();
    write.str("");
    write << setfill(SPACE) << setw(8) << hex << s_start;
    record += write.str();
    write.str("");
    write << setfill(SPACE) << setw(8) << hex << length << endl;
    record += write.str();
    write.str("");
    return record;
}
string SICXE_Parser::SymTabDefs(SICXE_Source section, uint32_t start) {
    string extDefStr, tmp;
    stringstream write;
    uint32_t location;
    bool found;
    for (int i = 0; i < section.extdef.size(); i++) {
        tmp = section.extdef.at(i) + SPACE;
        extDefStr = "";
        // search for extdef in instructions
        found = false;
        for (int j = 0;j < section.instructions.size() && !found;j++) { // every instruction of source
            if (section.instructions.at(j).label == tmp) {
                location = start + section.instructions.at(j).addr;
                write << setfill(SPACE) << setw(8);
                extDefStr += write.str();
                write.str("");
                write << setfill(SPACE) << setw(8) << tmp;
                extDefStr += write.str();
                write.str("");
                write << setfill(SPACE) << setw(8) << location << endl;
                extDefStr += write.str();
                write.str("");
                found = true;
            }
        }
        if (!found) { // definition never found in section
            errno = ENXIO;
            fprintf(stderr, "EXTDEF %s defined but not loaded", tmp.c_str());
            perror("");
        }
    }
    return extDefStr;
}
// pass argv raw arg for filepath and return the filename with no extention
string SICXE_Parser::RemoveFileExtension(string filename) {
    size_t start, end = 0;
    start = filename.find_last_of("/");
    if (start == 0) {
        start = filename.find_last_of("\\");
    }
    end = filename.find_last_of(".");
    filename = filename.substr(start + 1, end - start - 1);
    return filename;
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
        for (int j = 0;j < section.instructions.size() && !found;j++) { // every instruction of source
            if (section.instructions.at(j).label == tmp) {
                stream << setfill('0') << setw(ADDR_DIGIT_PLACES) << hex << section.instructions.at(j).addr;
                extDefRecStr += stream.str();
                stream.str("");
                found = true;
            }
        }
        if (!found) { // definition never found in section
            errno = ENXIO;
            fprintf(stderr, "EXTDEF %s defined but not loaded", tmp.c_str());
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
    SICXE_Instruction curInstruction;
    size_t idxForLength;
    int digitPlaces, bytecount, totalInstructions;
    uint32_t addrcount;
    bool isOverLimit = true;

    addrcount = section.start;
    bytecount = 0;
    totalInstructions = section.instructions.size();
    for (int i = 0;i < totalInstructions;i++) {
        curInstruction = section.instructions.at(i);
        if (isOverLimit) {// starts at overlimit true to set new TextRecord line
            addrcount += bytecount;
            bytecount = 0;
            isOverLimit = false;
            textRecStr += TEXTOBJ;
            stream << setfill('0') << setw(ADDR_DIGIT_PLACES) << hex << addrcount;
            textRecStr += stream.str();
            stream.str("");
            idxForLength = textRecStr.size();
        }
        // when in middle of parsing objcodes within limit of 16 bytes
        digitPlaces = ADDR_DIGIT_PLACES;
        if (curInstruction.mnemonic.find('+') != string::npos) // extended addressing case
            digitPlaces = EXT_ADDR_DIGIT_PLACES;

        if (bytecount + digitPlaces / 2 > TEXTREC_BYTE_LIMIT || i == totalInstructions - 1) {// check if surpassed byte limit or last instruction
            stream << hex << bytecount;
            textRecStr.insert(idxForLength, stream.str());
            textRecStr += "\n";
            stream.str("");
            isOverLimit = true;

            if (i != totalInstructions - 1)
                i--;
        }
        else { // within byte limit, add objcode
            bytecount += digitPlaces / 2;
            stream << setfill('0') << setw(digitPlaces) << hex << curInstruction.objcode;
            textRecStr += stream.str();
            stream.str("");
        }
    }
    return textRecStr;
}

string SICXE_Parser::BuildModRecord(int idx) {
    string modRecStr;
    stringstream stream;
    SICXE_Source section = sections.at(idx);
    SICXE_Instruction curInstruction;
    char sign;
    int idx_extref, modSize, totalInstructions = section.instructions.size();

    for (int i = 0;i < totalInstructions;i++) {// for every instruction in this section
        curInstruction = section.instructions.at(i);

        if (curInstruction.args.size() > 0) { // if this instruction has args
            for (int j = 0; j < curInstruction.args.size();j++) { // for every arg
                if ((idx_extref = HasExtRef(curInstruction.args.at(j), idx)) != FAIL_FIND) { // current arg is an extref
                    modSize = DEFAULT_MOD_SIZE;
                    modRecStr += MODOBJ;
                    sign = LeadingPlusOrMinusCheck(curInstruction.args.at(j));
                    if (curInstruction.mnemonic.front() == PLUS)
                        stream << setfill('0') << setw(ADDR_DIGIT_PLACES) << hex << curInstruction.addr + 1;
                    else
                        stream << setfill('0') << setw(ADDR_DIGIT_PLACES) << hex << curInstruction.addr;

                    modRecStr += stream.str();
                    stream.str("");

                    if (curInstruction.mnemonic == M_WORD)
                        modSize = WORD_MOD_SIZE;
                    stream << setfill('0') << setw(2) << hex << modSize;
                    modRecStr += stream.str();
                    stream.str("");
                    modRecStr += sign + section.extref.at(idx_extref) + "\n";
                }
            }
        }
    }
    return modRecStr;
}

// checks if the token is in the extrefs of sections[idx] and returns the idx
int SICXE_Parser::HasExtRef(string token, int sectionIdx) {
    bool found = false;
    int extrefIdx = -1;
    for (int i = 0;i < sections.at(sectionIdx).extref.size() && !found;i++) {
        if (token.find(sections.at(sectionIdx).extref.at(i)) != string::npos) {
            found = true;
            extrefIdx = i;
        }
    }
    return extrefIdx;
}

char SICXE_Parser::LeadingPlusOrMinusCheck(string token) {
    char opchar;

    if (token.front() == PLUS) // explicit plus leading
        opchar = PLUS;
    else if (token.front() == MINUS) // explicit minus leading
        opchar = MINUS;
    else // anything without leading operator assumed plus
        opchar = PLUS;

    return opchar;
}

