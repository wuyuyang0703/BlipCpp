#include <unordered_map>
#include <string>

enum TokenType {
  COMMAND, NUMBER, OPERATOR, STRING
};

enum CommandType {
  TEXT, OUT, VAR, SET
}

enum ErrorType {
    NOERR, FILEERR, COMMANDERR, ARGERR
}

const std::unordered_map<std::string, CommandType> CommandMap = {
    {"text", TEXT},
    {"out", OUT},
    {"var", VAR},
    {"set", SET}
};

class BlipToken {
public:
  std::string data;
  TokenType type;
  BlipToken();
  BlipToken(std::string data);
};

class BlipParser {
private:
    std::unordered_map<std::string, int> symbols;
    int calculate(std::string expression);
public:
    ErrorType loadFile(std::string fileName);
};
