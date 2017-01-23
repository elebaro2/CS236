#ifndef DATABASE_H
#define DATABASE_H
#include <iostream>
#include <map>
#include <sstream>

#include "relation.h"
#include "predicate.h"
#include "tuple.h"

/*Database class
 *Iterates through each query, creates new relations and
 *performs select, project and rename*/

using namespace std;

class Database{
public:
    Database(){}
    ~Database(){}

    void addRelation(Relation r){
        rMap.insert(pair<string,Relation>(r.getName(), r));
    }
    /*Iterates through queries, creates relation, matching it with the Schema it corresponds to*/
    void runQueries(){
        Relation temp;
        for (unsigned int i = 0; i < queries.size(); i++){
            string name = queries[i].getID();
            vector<Parameter> params = queries[i].getParameters();
            /* finds the relation that matches the query*/
            Relation match = rMap.find(name)->second;
            temp = match;
            /*loops through query params*/
            for(unsigned int j = 0; j < params.size(); j++){
                string type = params[j].getType();
                string value = params[j].getValue();
                /*checks to see what type of select to use*/
                if(type == "STRING")
                    temp = temp.select(j, value);
                if(type == "ID"){
                    bool is_dup = false;
                    int dupPosition = 0;
                    for (unsigned int k = 0; k < IDs.size(); k++){
                        if(IDs[k] == value){
                            is_dup = true;
                            dupPosition = k;
                        }
                    }
                    if((is_dup)){

                        temp = temp.select(dupPosition, j);
                    }else{
                        IDs.push_back(value);
                        positions.push_back(j);
                    }
                }
            }
            temp = temp.project(positions);
            temp = temp.rename(IDs);
            stringstream ss;
            ss << printParams(params);
            int numb = temp.countToPresentTuples(positions, IDs);
            cout << name << "(" << ss.str() << ")?";
            doesWork(temp.tuples.size(), numb );
            temp.presentTuples(positions, IDs, numb);
            IDs.clear();
            positions.clear();
        }
    }
    /*checks if relation has tuples*/
    void doesWork(int x, int numb){
        if(x > 0){
            cout << " Yes(" << numb << ")" << endl;
            return;
        }
        cout << " No" << endl;
    }
    /*prints the parameters of the relation*/
    string printParams(vector<Parameter> params){
        stringstream ss;
        for(unsigned int z = 0; z < params.size(); z++){
            ss << params[z].toString();
            if(z == (params.size() - 1)){}
            else ss << ",";
        }
        return ss.str();
    }

    /*creates lists of schemes, facts and queries and creates their corresponding tuples*/

    void create(vector<Predicate> s, vector<Predicate> f, vector <Predicate> q){
        schemes = s;
        facts = f;
        queries = q;
        for(unsigned int i = 0; i < s.size(); i++){
            Relation r;
            string name = s[i].getID();
            vector<Parameter> ps = s[i].getParameters();    /**/

            Scheme sch;
            sch.addParams(ps);
            r.setName(name);
            r.setSchema(sch);
            addRelation(r);
        }

        for(unsigned int i = 0; i < f.size(); i++){
            Tuple t;
            string factName = f[i].getID();
            vector<Parameter>factParams = f[i].getParameters();
            vector<string> factStrings;
            for(unsigned int j = 0; j < factParams.size(); j++){
                string hey = factParams[j].getValue();
                factStrings.push_back(hey);
        }
            t.List = factStrings;
            t.setRelationName(factName);
            addToRelations(t);
        }
    }
    void toString(){
        for( unsigned int i = 0; i < toPrint.size(); i++){
            toPrint[i].RelationtoString();
        }
    }

    /*adds relation to the relation map*/
    void addToRelations(Tuple t){
        string name = t.getRelationName();
        Relation curr = rMap.find(name)->second;
        curr.addTuple(t);
        rMap.at(name) = curr;
    }

    void printRelations(){
        cout << "Relations: " << endl;
        for(map<string, Relation>::iterator it = rMap.begin(); it!= rMap.end(); it++){
            cout << it->first << " => "<< endl;
            it->second.RelationtoString();
        }
    }

    map<string, Relation> rMap;
    vector <Relation> relations;
    vector <Predicate> schemes;
    vector <Predicate> facts;
    vector <Predicate> queries;
    vector <Relation> toPrint;
    vector <int> positions;
    vector <string> IDs; /*list of IDs*/

};

#endif // DATABASE_H

