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

    string getValue() const {
        return value;
    }

    string getType() const {
        return type;
    }

    string toString() const{
        return getValue();
    }

    bool operator <( const Parameter& p) const{
        return value < p.value;
    }

};

#endif // PARAMETER

