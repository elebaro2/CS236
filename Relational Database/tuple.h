#ifndef TUPLE_H
#define TUPLE_H
#include <iostream>
#include <vector>
#include "parameter.h"

using namespace std;
/*creates a tuple class in which the name of the relation is saved, along with the corresponding tuples, which are saved in a list*/
class Tuple{
private:

public:

    Tuple(){}
    ~Tuple(){}

    vector <string> List;
    string relationName;
    void setRelationName(string n){
        relationName = n;
    }

    string getRelationName(){
        return relationName;
    }

    void setTupleList(vector<string> list){
        List = list;
    }

    void printTuple(){

        for (unsigned int i = 0; i < List.size(); i++){
            if((i % 2 == 0)){
                cout << "\n";
            }
            cout << List[i];
        }
        cout << "\n" << endl;
    }

    bool operator< (const Tuple b) const {
        return List < b.List;
    }
};

#endif // TUPLE_H

