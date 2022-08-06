#include <unordered_map>
#include <string>

enum TokenType {
  COMMAND, NUMBER, OPERATOR, LETTER, STRING
};

enum CommandType {
  TEXT, OUT, VAR, SET
};

enum ErrorType {
    NOERR, FILEERR, COMMANDERR, ARGERR
};

enum OperatorType {
    ADD, SUB, MUL, DIV, MOD, AND, OR, LT, GT, EQ, NE, LE, GE, NOT, OPP
};

const std::unordered_map<std::string, CommandType> CommandMap = {
    {"text", TEXT},
    {"out", OUT},
    {"var", VAR},
    {"set", SET}
};

const std::unordered_map

class BlipToken {
private:
    bool isNumber() {
        for (std::string::iterator itr = data.begin(); itr != data.end(); ++itr) {
            if (*itr > '9' || *itr < '0') return false;
        }
        return true;
    }
    bool isLetter() {
        for (std::string::iterator itr = data.begin(); itr != data.end(); ++itr) {
            if (*itr > 'z' || *itr < 'A') return false;
        }
        return true;
    }
    bool isOperator() {
        if
    }
public:
    std::string data;
    BlipToken();
    BlipToken(std::string data);
    TokenType type();
    friend std::istream& operator>> (std::istream& is, BlipToken& token);

class BlipParser {
private:
    std::unordered_map<std::string, int> symbols;
    int calculate(std::string expression);
public:
    ErrorType loadFile(std::string fileName);
};
