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
                        if ((isspace(c) && token.size() != 0)) { // word defining condition for in general
                            if (mnemonicsDictionary.mnemonics.count(token) > 0U) { //check if token is in dictionary
                                inDictionary = true;
                            }

                            if (token.compare("END") == 0 || end) { //First checks for "END"
                                if(token.compare("END") == 0)
                                    curInstruction.mnemonic = token;
                                for (int i = 0; i < curSection.instructions.size();++i) {
                                    if (token.compare(curSection.instructions[i].label) == 0) { //Checks all labels to grab address
                                        curInstruction.addr = curSection.instructions[i].addr;
                                    }
                                }

                                token.erase();
                                end = true;
                                continue; //Move on to name after "END", end is false if next line
                            }
                            else if (curInstruction.addr == NULL) {        //Handles adding addr token
                                curInstruction.addr = stringToHex(token);
                                token.erase();
                            }
                            else if (curInstruction.label.compare("NULL") && curInstruction.mnemonic.compare("NULL") && !inDictionary) { //handles adding label token
                                curInstruction.label = token;
                                token.erase();
                            }
                            else if (curInstruction.mnemonic.compare("NULL")) { //Handles adding mnemonic token
                                curInstruction.mnemonic = token;
                                token.erase();
                            }
                            else if (curInstruction.args.empty()) { //Handles adding args (1st section if +/-)(2nd section if ',')(3rd if jusst token)
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
                                // Checks if token has comma and seperates and adds them to args
                                else if (token.find(',') != std::string::npos) {
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
                            else if (curInstruction.objcode == NULL && j == line.length() - 1) { //Handles adding object code tokens
                                curInstruction.objcode = stringToHex(token);
                                token.erase();
                            }
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
        //Find end address
        int sizeOfVector = curSection.instructions.size();
        if(curSection.instructions.at(sizeOfVector-1).mnemonic.compare("END") == 0){ //Check if the last instruction is END
            curSection.end = curSection.instructions.at(sizeOfVector-2).addr; //Get address of instruction before END
            curSection.end += 3;
        }
        else{
            curSection.end = curSection.instructions.at(sizeOfVector).addr; //Last instruction address
            curSection.end += 3;
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
        outfile.open(sections.at(i).filename, fstream::out);
        record += BuildHeaderRecord(i);
        record += BuildExtDef(i);
        record += BuildExtRef(i);
        record += BuildTextRecord(i);
        record += BuildModRecord(i);
        outfile.close();
    }
}

void SICXE_Parser::WriteSymTabFile() {
    stringstream write;
    string record = "";
    string filename = "ESTAB.st";
    outfile.open(filename, fstream::out);
    SICXE_Source source;
    string s_name;
    uint32_t s_start;
    uint32_t s_end;
    uint32_t length;
    
    for(int i=0; i < sections.size(); ++i){
        source = sections.at(i);
        s_name = sections.at(i).name;
        if(i == 0){
            s_start = sections.at(i).start; //000000
            s_end = sections.at(i).end; // 002F09
            length = s_end - s_start; // 002F09
            record += SymTabSections(s_name, s_start, s_end);
            record += SymTabDefs(source, length);
            continue;
        }
        s_start = s_end; //Next section starting address is the end of the previous
        length = sections.at(i).end - sections.at(i).start; 
        s_end = s_start + length; 
        record += SymTabSections(s_name, s_start, s_end);
        record += SymTabDefs(source, length);
    }
}
string SICXE_Parser::SymTabSections(string s_name, uint32_t s_start, uint32_t length){
    stringstream write;
    string record;
    write << setfill(SPACE) << setw(8) << s_name ;
    record += write.str();
    write.clear();
    write << s_start << setw(8);
    record += write.str();
    write.clear();
    write << length << setw(8) << endl;
    record += write.str();
    write.clear();
    return record;
}
string SICXE_Parser::SymTabDefs(SICXE_Source section, uint32_t start){
    string extDefStr, tmp;
    stringstream write;
    uint32_t location;
    bool found;
    for(int i=0; i < section.extdef.size(); ++i){
        tmp = section.extdef.at(i) + SPACE;
        extDefStr = "";
        // search for extdef in instructions
        found = false;
        for (int j = 0;j < section.instructions.size() && !found;i++) { // every instruction of source
            if (section.instructions.at(j).label == tmp && section.instructions.at(j).mnemonic == "EQU") {
                location = start + section.instructions.at(i).addr;
                write << setfill(SPACE) << setw(8);
                extDefStr+= write.str();
                write.clear();
                write << setfill(SPACE) << setw(8) << extDefStr;
                extDefStr += write.str();
                write.clear();
                write << setfill(SPACE) << setw(8) << location << endl;
                extDefStr += write.str();
                write.clear();
                found = true;
            }
        }
        if (!found) { // definition never found in section
            errno = ENXIO;
            fprintf(stderr, "EXTDEF %s defined but not loaded", tmp);
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
            stream.clear();
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
            stream.clear();
            isOverLimit = true;

            if (i != totalInstructions - 1)
                i--;
        }
        else { // within byte limit, add objcode
            bytecount += digitPlaces / 2;
            stream << setfill('0') << setw(digitPlaces) << hex << curInstruction.objcode;
            textRecStr += stream.str();
            stream.clear();
        }
    }
    return textRecStr;
}