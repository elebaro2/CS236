#ifndef DATABASE_H
#define DATABASE_H
#include <iostream>
#include <map>
#include <sstream>

#include "relation.h"
#include "predicate.h"
#include "tuple.h"
#include "rule.h"
#include "parameter.h"

/*Database class
 *Iterates through each query, creates new relations and
 *performs select, project and rename*/

using namespace std;

class Database{
public:
    Database(vector<Predicate> s, vector<Predicate> f, vector<Predicate> q, vector<Rule> r){
        create(s, f, q, r);
    }

    ~Database(){}

    void addRelation(Relation& r){
        rMap.insert(pair<string,Relation>(r.getName(), r));
    }

    int getSize(){
        map<string,Relation>::iterator it;
        int size = 0;
        for(it = rMap.begin(); it != rMap.end(); it++){
            int x = it->second.tuples.size();
            size = size + x;
        }
        return size;
    }

    /*Iterates through queries, creates relation, matching it with the Schema it corresponds to*/
    void runQueries(){
        doRules(rules);
        cout << "Schemes populated after " <<
                numpass << " passes through the Rules.\n";
        //cout << " do rules \n";
        Relation temp;
        for (size_t i = 0; i < queries.size(); i++){
            string name = queries[i].getID();
            vector<Parameter> params = queries[i].getParameters();
            /* finds the relation that matches the query*/
            Relation match = rMap.find(name)->second;
            temp = match;

            /*loops through query params*/
            for(size_t j = 0; j < params.size(); j++){
                string type = params[j].getType();
                string value = params[j].getValue();
                /*checks to see what type of select to use*/
                if(type == "STRING")
                    temp = temp.select(j, value);
                if(type == "ID"){
                    bool is_dup = false;
                    int dupPosition = 0;
                    for (size_t k = 0; k < IDs.size(); k++){
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
            cout << name << "(" << ss.str() << ")?";
            doesWork(temp.tuples.size(), temp );
            temp.presentTuples(positions, IDs);
            IDs.clear();
            positions.clear();
        }
    }

    void print_map(){
        cout << "This is the Map: \n";
        for(map<string, Relation>::iterator i = rMap.begin(); i != rMap.end(); i++){
            cout << "Strings: " << i->first << "  \nRelations: "; i->second.RelationtoString(); cout << endl;
        }
    }

    /*checks if relation has tuples*/
    void doesWork(int x, Relation &temp){
        if(x > 0){
            cout << " Yes(" << temp.countToPresentTuples(IDs) << ")" << endl;
            return;
        }
        cout << " No" << endl;
    }
    /*prints the parameters of the relation*/
    string printParams(vector<Parameter>& params){
        stringstream ss;
        for(size_t z = 0; z < params.size(); z++){
            ss << params[z].toString();
            if(z == (params.size() - 1)){}
            else ss << ",";
        }
        return ss.str();
    }

    /*creates lists of schemes, facts and queries and creates their corresponding tuples*/

    void create(vector<Predicate>& s, vector<Predicate>& f, vector<Predicate>& q, vector<Rule>& r){
        schemes = s;
        facts = f;
        queries = q;
        rules = r;
        for(size_t i = 0; i < s.size(); i++){
            Relation r;
            string name = s[i].getID();
            vector<Parameter> ps = s[i].getParameters();    /**/

            Scheme sch;
            sch.addParams(ps);
            r.setName(name);
            r.setSchema(sch);
            addRelation(r);
        }

        for(size_t i = 0; i < f.size(); i++){
            Tuple t;
            f[i].toString();
            string factName = f[i].getID();
            vector<Parameter>factParams = f[i].getParameters();
            vector<string> factStrings;
            for(size_t j = 0; j < factParams.size(); j++){
                string hey = factParams[j].getValue();
                factStrings.push_back(hey);
            }
            t.List = factStrings;
            t.setRelationName(factName);
            addToRelations(t);
        }
    }

    void toString(){
        for( size_t i = 0; i < toPrint.size(); i++){
            toPrint[i].RelationtoString();
        }
    }

    /*adds relation to the relation map*/
    void addToRelations(Tuple& t){
        rMap[t.relationName].addTuple(t);
    }

    void addRToRelations(Relation& r){
            for(set<Tuple>::iterator it = r.tuples.begin(); it != r.tuples.end(); it++){
                Tuple temp = *it;
                if(!temp.List.empty())
                    addToRelations(temp);
            }
    }

    void printRelations(){
        cout << "Relations: " << endl;
        for(map<string, Relation>::iterator it = rMap.begin(); it!= rMap.end(); it++){
            cout << it->first << " => "<< endl;
            it->second.RelationtoString();
        }
    }

    void doRules(vector<Rule>& rules){
        numpass = 0;
        if(rules.empty())
            return;
        int sizeBefore = 0;
        bool keepgoing = true;
        while(keepgoing){
            sizeBefore = getSize();
            evalRules(rules);
            numpass++;
            if(sizeBefore == getSize()){
                keepgoing = false;
            }
        }
        //printRelations
    }

    Relation evalParams(vector<Predicate>& q, size_t i, Relation& source){
        size_t index = i;
        vector<Parameter> params = q[index].paramList;
        for(size_t j = 0; j < params.size(); j++){
            string type = params[j].getType();
            string value = params [j].getValue();
            if(type == "STRING"){
                source = source.select(j, value);
            }
            if(type == "ID"){
                bool is_dup = false;
                int dupPosition = 0;
                for(size_t k = 0; k < IDs.size(); k++){
                    if(IDs[k] == value){
                        is_dup = true;
                        dupPosition = k;
                    }
                }
                if((is_dup))
                    source = source.select(dupPosition, j);
                else{
                    IDs.push_back(value);
                    positions.push_back(j);
                }
            }
        }
        source = source.project(positions);
        source = source.rename(IDs);
        IDs.clear();
        positions.clear();
        return source;
    }

    void evalQueries(vector<Predicate>& currentQ, size_t x){
        size_t i;
        Relation temp, relToAdd;
        for(i = 0; i < currentQ.size(); i++){
            temp = rMap[currentQ[i].getID()];
            relToAdd = evalParams(currentQ, i , temp);
            newFacts.push_back(relToAdd);
        }
        Relation projected, unioned, toUnion;

        if(newFacts.size() == 1){
            projected = newFacts.front().project(rules[x].predicate.paramList);
            toUnion = rMap[rules[x].predicate.id];
            unioned = projected.UnionWith(toUnion,projected);
            addRToRelations(unioned);
        }
        else if(newFacts.size() > 1){
            evalRulesNewFacts(x);
            //add_facts();
        }
    }

    void evalRules(vector<Rule>& rules){
        for(size_t i = 0; i < rules.size(); i++){
            evalQueries(rules[i].predicateList, i);
            newFacts.clear();
        }
    }

    void add_facts(){
        for(size_t i = 0; i < newFacts.size(); i++){
                addRToRelations(newFacts[i]);
        }
    }
    void evalRulesNewFacts(size_t i){
        Relation one, three, projected, toUnion, unioned;

        for(size_t j = 0; j < newFacts.size(); j++){

            Relation temp1 = newFacts[j];
            if(one.tuples.empty()){
                if(j < newFacts.size() - 1){
                    j++;
                    Relation temp2 = newFacts[j];
                        one = one.join(temp1, temp2);
                    if(j == newFacts.size() - 1){
                        //cout << "rule "<< rules[i].toString();
                        projected = one.project(rules[i].predicate.paramList);
                        //cout << "projected "; projected.RelationtoString();
                        toUnion = rMap[rules[i].predicate.id];
                        unioned = projected.UnionWith(toUnion, projected);
                        addRToRelations(unioned);
                    }
                }
            }
            else{
                three = three.join(one, temp1);
                one = three;
                if(one.tuples.empty())
                    return;
                if(j == newFacts.size() - 1){
                    projected = one.project(rules[i].predicate.paramList);
                    toUnion = rMap[rules[i].predicate.id];
                    unioned = projected.UnionWith(toUnion, projected);
                    addRToRelations(unioned);
                }
            }
        }
    }

    Relation find(string& n){
        Relation r;
        r = rMap.find(n)->second;
        return r;
    }
    int numpass;
    map<string, Relation> rMap;
    vector <Predicate> schemes;
    vector <Predicate> facts;
    vector <Predicate> queries;
    vector <Relation> toPrint;
    vector <int> positions;
    vector <string> IDs; /*list of IDs*/
    vector <Rule> rules;
    vector <Relation> newFacts;

};

#endif // DATABASE_H

