#include "BlipParser.h"
#include <fstream>
#include <iostream>
#include <sstream>

BlipToken::BlipToken() {
    data = "";
    type = STRING;
}

BlipToken::BlipToken(std::string data) {
    this->data = data;
}

TokenType BlipToken::type() {
    if (isCommand()) return COMMAND;
    if (isNumber()) return NUMBER;
    if (isLetter()) return LETTER;
    if (isOperator()) return OPERATOR;
    return STRING;
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
    BlipToken firstToken;
    std::streampos orig = ifs.tellg();
    CommandType currentCommand;
    std::string currentVariable;
    while (ifs >> firstToken) {
        if (firstToken.isCommand()) {
            if (currentCommand != TEXT) result = calculate();
            switch (currentCommand) {
                case OUT:
                    std::cout << result << std::endl;
                case VAR:
                case SET:
                    symbols[currentVariable] = result;
            }
            std::stack<BlipToken>().swap(expressionCache);
            currentCommand = CommandMap.at(firstToken.data);
            if (currentCommand == VAR) {
                ifs >> currentVariable;
                if (symbols.find(currentVariable) != symbols.end()) {
                    std::cout << "Variable " << currentVariable << " incorrectly reinitialized" << std::endl;
                }
            }
            if (currentCommand == SET) {
                ifs >> currentVariable;
                if (symbols.find(currentVariable) != symbols.end()) {
                    std::cout << "Variable " << currentVariable << " not declared" << std::endl;
                }
            }
            ifs >> std::ws;
        }
        else {
            ifs.seekg(orig, std::ios::beg);
            switch (currentCommand) {
                case TEXT: {
                    char textHeader;
                    std::string textContent;
                    ifs.peek(textHeader);
                    if (textHeader == '\"') {
                        ifs.seekg(1, std::ios::cur);
                        getline(ifs, textContent, '\"');
                    }
                    else {
                        ifs >> textContent;
                    }
                    size_t commentPos = textContent.find("//");
                    if (commentPos != std::string::npos) {
                        textContent = textContent.substr(0, commentPos);
                    }
                    std::string dump;
                    getline(ifs, dump);
                    std::cout << formatText(textContent);
                    break;
                }
                case OUT:
                case VAR:
                case SET: {
                    BlipToken token;
                    ifs >> token;
                    size_t commentPos = token.data.find("//");
                    if (commentPos != std::string::npos) {
                        if (token.data.substr(0, 2) != "//") {
                            token.data = token.data.substr(0, commentPos);
                            expressionCache.push(token);
                            std::string dump;
                            getline(ifs, dump);
                        }
                    }
                    else expressionCache.push(token);
                    break;
                }
            }
        }
    } 
}

std::string BlipParser::formatText(std::string textContent) {
    bool specChr = false;
    std::string retVal;
    for (std::string::iterator itr = textContent.begin(); itr != textContent.end(); ++i) {
        if (specChr) {
            switch (*itr) {
                case '\\':
                    retVal.push_back('\\');
                    break;
                case 'n':
                    retVal.push_back('\n');
                    break;
                case 't':
                    retVal.push_back('\t');
                    break;
                default:
                    retVal.push_back('\\');
                    retVal.push_back(*itr);
            }
            specChr = false;
        }
        else if (*itr == '\\') specChr = true;
        else retVal.push_back(*itr);
    }
    return retVal;
}

int BlipParser::calculate() {
    std::stack<int> calcSequence;
    while (!expressionCache.empty()) {
        BlipToken token = expressionCache.front();
        expressionCache.pop();
        if (token.isNumber()) {
            calcSequence.push(stoi(token.data));
        }
        else if (token.isLetter()) {
            if (symbols.find(token.data) == symbols.end()) {
                calcSequence.push(0);
            }
            else calcSequence.push(symbols.at(token.data));
        }
        else if (token.isOperator()) {
            int first = calcSequence.front();
            calcSequence.pop();
            if (token.data != "~" && token.data != "!") {
                int second = calcSequence.front();
                calcSequence.pop();
            }
            switch (OperatorMap.at(token.data)) {
                case ADD: {
                    calcSequence.push(first + second);
                    break;
                }
                case SUB: {
                    calcSequence.push(first - second);
                    break;
                }
                case MUL: {
                    calcSequence.push(first * second);
                    break;
                }
                case DIV: {
                    calcSequence.push(first / second);
                    break;
                }
                case MOD: {
                    calcSequence.push(first % second);
                    break;
                }
                case AND: {
                    int result = (first && second) ? 1 : 0;
                    calcSequence.push(result);
                    break;
                }
                case OR: {
                    int result = (first || second) ? 1 : 0;
                    calcSequence.push(result);
                    break;
                }
                case LT: {
                    int result = (first < second) ? 1 : 0;
                    calcSequence.push(result);
                    break;
                }
                case GT: {
                    int result = (first > second) ? 1 : 0;
                    calcSequence.push(result);
                    break;
                }
                case EQ: {
                    int result = (first == second) ? 1 : 0;
                    calcSequence.push(result);
                    break;
                }
                case NE: {
                    int result = (first != second) ? 1 : 0;
                    calcSequence.push(result);
                    break;
                }
                case LE: {
                    int result = (first <= second) ? 1 : 0;
                    calcSequence.push(result);
                    break;
                }
                case GE: {
                    int result = (first >= second) ? 1 : 0;
                    calcSequence.push(result);
                    break;
                }
                case NOT: {
                    int result = (!first) ? 1 : 0;
                    calcSequence.push(result);
                    break;
                }
                case OPP: {
                    calcSequence.push(0-first);
                    break;
                }
            }
        }
    }
    return calcSequence.front();
}
