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

ErrorType BlipParser::loadFile(std::string fileName) {
    std::ifstream(fileName) ifs;
    if (!ifs.is_open()) {
        std::cerr << "Cannot Open File" << std::endl;
        return FILEERR;
    }
    std::string command;
    std:::streampos orig = ifs.tellg();
    while (ifs >> command) {
    }
}
