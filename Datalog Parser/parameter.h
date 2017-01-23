#ifndef PARAMETER
#define PARAMETER
#include <string>
#include <sstream>
using namespace std;
/*holds a parameter and the type of parameter (string, id or expression)*/
class Parameter{
public:
    Parameter(){
        type = "";
        value = "";
    }

    Parameter(string tokenType, string val){
        type = tokenType;
        value = val;
    }

    ~Parameter(){}

    string type;
    string value;

    string getValue(){
        return value;
    }

    string getType(){
        return type;
    }

    string toString(){
        return getValue();
    }
};

#endif // PARAMETER

