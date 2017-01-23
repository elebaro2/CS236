#ifndef RELATION
#define RELATION
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <set>
#include "tuple.h"
#include "predicate.h"
#include "scheme.h"

using namespace std;

class Relation{
public:
    Relation(){}
    ~Relation(){}

    string name;
    Scheme schema;
    set<Tuple> tuples;

    string getName(){
        return name;
    }

    void setName(string s){
        name = s;
    }

    Scheme getSchema(){
        return schema;
    }

    void setSchema(Scheme s){
        schema = s;
    }

    /*adds tuples to the relation*/

    void addTuple(Tuple t){
        tuples.insert(t);
       /* cout << "\nadding tuple ";
                t.printTuple();
                cout << " to set ( " << tuples.size() << " )" << endl; */   }
    /*returns tuples for which rename and project were used in the relation*/
    void presentTuples(vector<int> positions, vector<string> IDs, int number){
        /*check each tuple within the relation*/
        for(set<Tuple>::iterator i = tuples.begin(); i != tuples.end(); i++){
            Tuple tup = *i;
            int numOut = 0;
            stringstream ss;
            /*out puts the contents of the tuple object*/
            for(unsigned int j = 0; j < tup.List.size(); j++){
                if((test(j,tup.List.size(),IDs.size()))){
                    if( j % IDs.size() == 1)
                        ss << IDs[j % positions.size()] << "=" << tup.List[j] << endl;

                else{
                    if((tryList(j,tup.List.size(),numOut)))
                        ss << IDs[j % positions.size()] << "=" << tup.List[j] << endl;
                    else
                        ss << "  " << IDs[j % positions.size()] << "=" << tup.List[j] << endl;
                    }
                }
                else{
                    int newNumb = (tup.List.size() / number);
                    if(numOut == (newNumb - 1)){
                        ss << IDs[j % positions.size()] << "="  << tup.List[j] << endl;
                        numOut = 0;
                    }
                    else{
                        if(numOut == 0){
                            ss << "  " << IDs[j % positions.size()] << "=" << tup.List[j] << ", ";
                            numOut++;
                        }
                    else{
                            ss << IDs[j % positions.size()] << "=" << tup.List[j] << ", ";
                            numOut++;
                        }
                    }
                }
            }
            cout << ss.str();
        }
    }

    bool test(int j, int list, int ids){
        if(j == list - 1 || ids < 2)
            return true;
        return false;
    }

    /*checks to see if it's at the end of the tuple's list*/
    bool tryList(int j, int size, int num){
        if(j == size-1 && num > 0)
            return true;
        return false;
    }
    /*counts how many tuples are within the relation*/
    int countToPresentTuples(vector<int> positions, vector<string> IDs){
        int amount = 0;
        if(tuples.size()){
        for(set<Tuple>::iterator i = tuples.begin(); i != tuples.end(); i++){
            Tuple tup = *i;
                for(unsigned int j = 0; j < tup.List.size(); j++){
                    amount++;
                }
            }
        }
        if(IDs.size() > 1)
            amount = amount/ IDs.size();
        if(amount == 0)
            return amount = 1;
        return amount;
    }
    /*Prints relations (used during testing)*/
    void RelationtoString(){
        cout << "Relation: " << endl << "name: " << name << endl;
        schema.print();
        cout<< "tuples ( " << tuples.size() << " )" << endl;
        for(set<Tuple>::iterator i = tuples.begin(); i != tuples.end(); i++){
            Tuple tup = *i;
            //tup.printTuple();
        }
    }
    /*select function for relation when using a STRING*/
    Relation select(int index, string value){
        Relation r;
        r.name = name;
        r.schema = schema;
        for(set<Tuple> :: iterator i = tuples.begin(); i != tuples.end(); i++){
            Tuple tup = *i;
            if(tup.List[index] == value){
                r.addTuple(tup);
            }
            //tup.printTuple();
        }
        return r;
    }
    /*select function for relation when using IDs*/
    Relation select(int index1, int index2){
        Relation r;
        r.name = name;
        r.schema = schema;

        for(set<Tuple>::iterator i = tuples.begin(); i != tuples.end(); i++){
            Tuple tup = *i;

            if(tup.List[index1] == tup.List[index2]){
                r.addTuple(tup);
            }
            //tup.printTuple();
        }
        return r;
    }
    /*Project function*/
    Relation project(vector<int> positions){
        Relation r;
        r.name = name;
        r.schema = schema;

        set<Tuple>::iterator it;
        Tuple projected;
        int toProject;

        bool shouldInsert = false;

        for(it = tuples.begin(); it != tuples.end(); it++){
            Tuple temp;
            temp = *it;

            shouldInsert = true;

            for(unsigned int i = 0; i < positions.size(); i++){
                toProject = positions.at(i);
                projected.List.push_back(temp.List.at(toProject));
            }
        }

        if(shouldInsert){
            r.tuples.insert(projected);
        }
        //r.RelationtoString();
        return r;
    }
    /*Project ( which column you want (vector of indices of columns)
     *      for each t in the tuple
     *        take the tuple and pull out certain columns*/

    /*Rename function*/
    Relation rename(vector<string> IDs){
        Relation r;
        r.name = name;
        r.tuples = tuples;
        if(IDs.size() != 0)
            r.schema.setParams(IDs);
        return r;
    }
    /**/

};

#endif // RELATION

