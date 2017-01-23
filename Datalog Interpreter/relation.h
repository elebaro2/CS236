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
    bool is_new(){
        if(schema.params.empty() && name == "" && tuples.empty())
            return true;
        return false;
    }

    string name;
    Scheme schema;
    set<Tuple> tuples;

    string getName() const {
        return name;
    }

    void setName(string s){
        name = s;
        for(set<Tuple>::iterator r = tuples.begin(); r != tuples.end(); r++){
            Tuple t = *r;
            t.setRelationName(s);
        }
    }

    Scheme getSchema() const {
        return schema;
    }

    void setSchema(Scheme s){
        schema = s;
    }

    void setSchemaParams(vector<Parameter>& params){
        schema.addParams(params);
    }

    int getTuplesSize() const {
        return tuples.size();
    }

    /*adds tuples to the relation*/

    void addTuple(Tuple& t){
        t.setRelationName(name);
        tuples.insert(t);
        /*cout << "\nname " <<name << "\nadding tuple ";
                t.printTuple();
                cout << " to set ( " << tuples.size() << " )" << endl;*/    }
    /*returns tuples for which rename and project were used in the relation*/
    void presentTuples(vector<int>& positions, vector<string>& IDs){
            /*check each tuple within the relation*/
            for(set<Tuple>::iterator i = tuples.begin(); i != tuples.end(); i++){
                Tuple tup = *i;
                int numOut = 0;
                stringstream ss;
                /*out puts the contents of the tuple object*/
                for(size_t j = 0; j < tup.List.size(); j++){
                    if((test(j,tup.List.size(),IDs.size()))){
                        if( j % IDs.size() == 1)
                            ss  <<IDs[j % positions.size()] << "=" << tup.List[j] << endl;

                    else{
                        if((tryList(j,tup.List.size(),numOut)))
                            ss << IDs[j % positions.size()] << "=" << tup.List[j] << endl;
                        else
                            ss << "  " << IDs[j % positions.size()] << "=" << tup.List[j] << endl;
                        }
                    }
                    else{
                        //int newNumb = (tup.List.size() / number);
                        int tupsize = tup.List.size();
                        if(numOut - 1 == tupsize){
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
    int countToPresentTuples(vector<string>& IDs){
        int amount = 0;
        if(tuples.size()){
        for(set<Tuple>::iterator i = tuples.begin(); i != tuples.end(); i++){
            Tuple tup = *i;
                for(size_t j = 0; j < tup.List.size(); j++){
                    amount++;
                }
            }
        }
        if(IDs.size() > 1)
            amount = amount/ IDs.size();
        if(amount == 0){
            return amount = 1;
        }
        return amount;
    }
    /*Prints relations (used during testing)*/
    void RelationtoString(){
        cout << "Relation: " << endl << "name: " << name << endl;
        schema.print();
        cout<< "tuples ( " << tuples.size() << " )" << endl;
        for(set<Tuple>::iterator i = tuples.begin(); i != tuples.end(); i++){
            Tuple tup = *i;
            tup.printTuple();
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
    Relation project(vector<int>& positions){
        Relation r;
        r.name = name;
        r.schema = schema;
        set<Tuple>::iterator it;
        Tuple projected;
        int toProject;
        Scheme s;
        bool toAdd = false;
        vector<Tuple> tempvec;
        for(it = tuples.begin(); it != tuples.end(); it++){
            Tuple temp = *it;
            toAdd = true;
            for(size_t i = 0; i < positions.size(); i++){
                toProject = positions[i];
                projected.List.push_back(temp.List[toProject]);
                s.addtoParams(schema.params[toProject]);
                //cout << "position, tuple list and param\n";
                //cout << positions[i] << " " << temp.List[toProject] << " " << schema.params[toProject].value << endl;
            }
            tempvec.push_back(projected);
            projected.List.clear();
        }
        if(toAdd){
            for(size_t j = 0; j < tempvec.size(); j++)
            r.tuples.insert(tempvec[j]);
        }
        r.setSchema(s);
        return r;
    }
    /*Project ( which column you want (vector of indices of columns)
     *      for each t in the tuple
     *        take the tuple and pull out certain columns*/

    Relation project(vector<Parameter>& paramList){
        Relation r;
        r.name = name;
        r.schema = schema;

        vector<int> index;
        //cout << "schema "; schema.print();
        //cout << "list size " << paramList.size() << "\n schema size" << schema.params.size() << endl;

        for(size_t i = 0; i < paramList.size(); i++){
            for(size_t j = 0; j < schema.params.size(); j++){
                if(paramList[i].value == schema.params[j].value){
                   index.push_back(j);
                }
                //c..out<< "param " << paramList[i].value << " schema " << schema.params[j].value << endl;
            }
        }
        //cout << "index size " << index.size() << endl;

        r = project(index);
        return r;
    }

    /*Rename function*/
    Relation rename(vector<string>& IDs){
        Relation r;
        r.name = name;
        r.tuples = tuples;
        if(IDs.size() != 0)
            r.schema.setParams(IDs);
        return r;
    }

    void rename(string s, size_t j){
        schema.rename(j,s);
    }

    Relation UnionWith(Relation& toUnion, Relation& projected){
        set<Tuple>::iterator it;
        for(it = projected.tuples.begin(); it != projected.tuples.end(); it++){
            Tuple temp = *it;
            toUnion.addTuple(temp);
        }
        return toUnion;
    }

    Relation join(Relation& one, Relation& two){
        Relation r;
        Scheme s;
        Tuple t;
        set<Tuple>::iterator it;
        set<Tuple>::iterator iter;

        s = join(one.schema, two.schema);
        r.schema = s;
        r.name = name;

        //cout << "relations to join one and two\n";
        //one.RelationtoString(); two.RelationtoString();

        for(it = one.tuples.begin(); it != one.tuples.end(); it++){
            Tuple t1 = *it;
            for(iter = two.tuples.begin(); iter != two.tuples.end(); iter++){
                Tuple t2 = *iter;
                if(joinable(one.schema, two.schema, t1, t2)){
                    t = join(one.schema, two.schema, t1,t2);
                    //cout << "tuple to join\n"; t.printTuple();
                    r.tuples.insert(t);
                    //cout << " scheme "; s.print();
                }
           }
        }
        return r;
    }

    Scheme join(Scheme s1, Scheme s2){
        bool toAdd;
        for(size_t i = 0; i < s2.params.size(); i++){
            toAdd = true;
            for(size_t j = 0; j < s1.params.size(); j++){
                if(s2.params[i].getValue() == s1.params[j].getValue()){
                    toAdd = false;
                    break;
                }
            }
            if(toAdd){
                s1.addtoParams(s2.params[i]);
            }
        }
        return s1;
    }

    Tuple join(Scheme& s1, Scheme& s2, Tuple& t1, Tuple& t2){
        Tuple t;
        for(size_t i = 0; i < t1.List.size(); i++){
            t.List.push_back(t1.List[i]);
        }
        /*cout << "current print";
        t.printTuple();
        cout << "join with";
        t2.printTuple();
        cout << "params used\n";
        s1.print();
        s2.print();*/
        bool toAdd;
        for(size_t j = 0; j < t2.List.size(); j++){
            toAdd = true;
            for(size_t z = 0; z < s1.params.size(); z++){

                if(s2.params[j].getValue() == s1.params[z].getValue()){
                    toAdd = false;
                }
            }
                if(toAdd)
                    t.List.push_back(t2.List[j]);

            }

        return t;
    }

    bool joinable(Scheme& s1, Scheme& s2, Tuple& t1, Tuple& t2){
        string name1, name2;
        string v1, v2;

        for(size_t i = 0; i < s1.params.size(); i++){
            name1 = s1.params[i].getValue();
            v1 = t1.List[i];
            for(size_t j = 0; j < s2.params.size(); j++){
                name2 = s2.params[j].getValue();
                v2 = t2.List[j];
                if(name1 == name2 && v1 != v2)
                    return false;
            }
        }
        return true;
    }
    bool operator<(const Relation r)const {
        return tuples < r.tuples;
    }
};

#endif // RELATION

