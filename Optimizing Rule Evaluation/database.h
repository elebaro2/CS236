#ifndef DATABASE_H
#define DATABASE_H
#include <iostream>
#include <map>
#include <sstream>
#include "node.h"
#include "relation.h"
#include "predicate.h"
#include "tuple.h"
#include "rule.h"
#include "parameter.h"
#include "graph.h"

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
        if(!rules.empty())
            createGraph();
        else
            cout << "Dependency Graph\nRule Evaluation\n";
        cout << "\nQuery Evaluation\n";
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
            int numb = temp.countToPresentTuples(IDs);
            cout << name << "(" << ss.str() << ")?";
            doesWork(temp.tuples.size(), numb );
            temp.presentTuples(positions, IDs, numb);
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
    void doesWork(int x, int numb){
        if(x > 0){
            cout << " Yes(" << numb << ")" << endl;
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
                cout << numpass << " passes: ";
                keepgoing = false;
            }
        }
        //printRelations();

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
            temp = rMap.find(currentQ[i].getID())->second;
            relToAdd = evalParams(currentQ, i , temp);
            newFacts.push_back(relToAdd);
        }
        Relation projected, unioned, toUnion;

        if(newFacts.size() == 1){
            projected = newFacts.front().project(rules[x].predicate.paramList);
            toUnion = find(rules[x].predicate.id);
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
                        toUnion = find(rules[i].predicate.id);
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
                    toUnion = find(rules[i].predicate.id);
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

    void createGraph(){
        Graph graph(rules);
        cout << graph.print_graph();
        eval_graph(graph);
    }
    bool repeatAgain (vector<Rule> rules){
        bool repeat = false;
        string head = rules[0].predicate.id;
        for(auto body:rules[0].predicateList){
            if(head == body.id)
                repeat = true;
        }
        return repeat;
    }

    void eval_graph(Graph & graph){
        vector <int> postorder_nums = graph.getHighestToLowestPON();
        map<int, set<int>> strongly_connected_components = graph.scc;
        cout << "Rule Evaluation\n";
        for(auto index : postorder_nums){
            stringstream ds;
            vector<Rule>evaluate;
            int count = 0;
            if(strongly_connected_components[index].size() > 0){
                for(set<int>::iterator i = strongly_connected_components[index].begin(); i != strongly_connected_components[index].end(); i++){
                    count++;
                    int rule_num = *i;
                    ds << "R" << rule_num;
                    evaluate.push_back(rules[rule_num]);
                    int indexn = strongly_connected_components[index].size();
                    if(count == indexn)
                        ds << "";
                    else
                        ds << ", ";
                }
            }

        doRules(evaluate);
        cout << ds.str();
        if(strongly_connected_components.at(index).size() > 0)
        cout << endl;
        }
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

