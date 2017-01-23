#ifndef TOKEN
#define TOKEN
#include <iostream>
#include <sstream>
using namespace std;


class token{

private:

        string value;
        string type;
        // token line number
        int lineNum;

public:
        token(string T, string V, int L){
            value = V;
            lineNum = L;
            type = T;
        }
        token(){
            value = "";
            lineNum =0;
            type = "";
        }

        string printTokens() const{
            stringstream ss;
            ss << "(" << type << ",\"" << value << "\"," << lineNum << ")\n";
            return ss.str();
        }

        string getValue() const {
            return value;
        }

        string getType() const {
            return type;
        }

};
#endif // TOKEN
