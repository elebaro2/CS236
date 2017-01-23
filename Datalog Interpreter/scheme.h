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
    void addParams(vector<Parameter>& p){
        params = p;
    }

    void addtoParams(Parameter& p){
        params.push_back(p);
    }

    void setParams(vector<string>& p){
        for (size_t i = 0; i < p.size(); i++){
            Parameter newP("ID", p[i]);
            params.push_back(newP);
        }
    }

    void rename (size_t pos, string s){
        Parameter p("ID", s);
        params.at(pos) = p;
    }

    void print(){
        for(size_t i = 0; i < params.size(); i++)
            cout << params[i].toString() << endl;
    }

    vector<Parameter> getParams() const {
        return params;
    }
    bool operator<(Scheme s) const{
        return params > s.params;
    }

};

#endif // SCHEME

