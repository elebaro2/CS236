#ifndef PREDICATE
#define PREDICATE
#include "parameter.h"
#include <vector>

using namespace std;
/*stores predicates with id and the list of parameters*/
class Predicate{
public:

    Predicate(){
        id = "";
        paramList = vector<Parameter>();
    }
    Predicate(string ident){
        id = ident;
        paramList = vector<Parameter>();
    }
    Predicate(string ident, vector <Parameter> parList){
        id = ident;
        paramList = parList;
    }

    ~Predicate(){}

    vector <Parameter> paramList;

    string id;

    string getID(){
        return id;
    }

    vector<Parameter> getParameters(){
        return paramList;
    }

    void addParameter(Parameter param){
        paramList.push_back(param);
    }

    string toString(){
        stringstream ss;
        ss << id << "(";
        for(unsigned int i = 0; i < paramList.size(); i++){
            if(i == 0)
                ss << paramList[i].toString();
            else
                ss << "," << paramList[i].toString();
        }
        ss << ")";
        return ss.str();
    }
};

#endif // PREDICATE

