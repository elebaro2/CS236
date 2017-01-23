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

        string printTokens(){
            stringstream ss;
            ss << "(" << type << ",\"" << value << "\"," << lineNum << ")\n";
            return ss.str();
        }

};
#endif // TOKEN
