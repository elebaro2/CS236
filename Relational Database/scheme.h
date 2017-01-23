#ifndef SCHEME
#define SCHEME
#include <vector>
#include <iostream>
#include "parameter.h"

using namespace std;
/*creates a list of parameters, which are the schemes the program tests the queries against*/

class Scheme{
public:
    Scheme (){}
    ~Scheme(){}

    vector <Parameter> params;
    void addParams(vector<Parameter> p){
        params = p;
    }

    void setParams(vector<string> p){
        for (unsigned int i = 0; i < p.size(); i++){
            Parameter newP("ID", p[i]);
            params.push_back(newP);
        }
    }

    void rename (int pos, string s){
        Parameter p("ID", s);
        params.at(pos) = p;
    }

    void print(){
        for(unsigned int i = 0; i < params.size(); i++)
            cout << params[i].toString() << endl;
    }

    vector<Parameter> getParams(){
        return params;
    }
};

#endif // SCHEME

