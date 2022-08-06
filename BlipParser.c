#include "BlipParser.h"
#include <fstream>
#include <iostream>

BlipToken::BlipToken() {
    data = "";
    type = STRING;
}

BlipToken::BlipToken(std::string data) {
    this->data = data;
}

TokenType BlipToken::type() {
    if (CommandMap.find(data) != CommandMap.end()) {
        return COMMAND;
    }
}

std::istream& operator>> (std::istream& is, BlipToken& token) {
    is >> token.data;
    return is;
}

ErrorType BlipParser::loadFile(std::string fileName) {
    std::ifstream(fileName) ifs;
    if (!ifs.is_open()) {
        std::cerr << "Cannot Open File" << std::endl;
        return FILEERR;
    }
    BlipToken command;
    std:::streampos orig = ifs.tellg();
    while (ifs >> command) {
        
    } 
}
