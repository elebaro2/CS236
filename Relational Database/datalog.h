#ifndef DATALOG
#define DATALOG
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdio>
#include <set>

#include "token.h"
#include "predicate.h"
#include "rule.h"

using namespace std;

class datalog{
public:
    datalog(){
        schemes = vector<Predicate>();
        facts = vector<Predicate>();
        rules = vector<Rule>();
        queries = vector<Predicate>();
        domain = set<string>();
    }

    ~datalog(){}
    /*vectors hold the pieces of each intial part of
    *the grammar Ex. Schemes are put into the schemes
    *vector, etc.*/
    vector<Predicate> schemes;
    vector<Predicate> facts;
    vector<Rule> rules;
    vector<Predicate> queries;
    set<string> domain;

    vector<Predicate> getSchemes(){
        return schemes;
    }

    vector<Predicate> getQueries(){
        return queries;
    }

    vector<Predicate> getFacts(){
        return facts;
    }

    /*adds to the correct buckets and uses the correct parameters*/
    void addScheme(Predicate s){
        schemes.push_back(s);
    }
    void addFact(Predicate f){
        facts.push_back(f);
        addDomain(f.getParameters());
    }
    void addRule(Rule r){
        rules.push_back(r);
        addDomain(r.getPredicate().getParameters());
    }
    void addQuery(Predicate q){
        queries.push_back(q);
    }
    void addDomain(vector<Parameter> pList){
        for(unsigned int i = 0; i < pList.size(); i++){
            if(pList[i].getType() == "STRING")
                domain.insert(pList[i].getValue());
        }
    }
    /*prints out the valid grammar input*/
    string stringSchemes(){
        stringstream ss;
        ss << "Schemes(" << schemes.size() << "):";
        for(unsigned int i = 0; i < schemes.size(); i++)
            ss << "\n  " << schemes[i].toString();
        return ss.str();
    }
    string stringFacts(){
        stringstream ss;
        ss << "Facts(" << facts.size() << "):";
        for(unsigned int i = 0; i < facts.size(); i++)
            ss << "\n  " << facts[i].toString() << ".";
        return ss.str();
    }
    string stringRules(){
        stringstream ss;
        ss << "Rules(" << rules.size() << "):";
        for(unsigned int i = 0; i < rules.size(); i++)
            ss <<"\n  "<< rules[i].toString();
        return ss.str();
    }
    string stringQueries(){
        stringstream ss;
        ss << "Queries(" << queries.size() << "):";
        for(unsigned int i = 0; i < queries.size(); i++)
            ss << "\n  " << queries[i].toString() << "?";
        return ss.str();
    }
    string stringDomain(){
        stringstream ss;
        ss << "Domain(" << domain.size() << "):";
        set<string>::iterator mItr;
        for(mItr = domain.begin(); mItr != domain.end(); mItr++)
            ss << "\n  " << (*mItr);
        return ss.str();
    }
    string toString(){
        stringstream ss;
        ss << "Success!\n";
        ss << stringSchemes() << endl <<
              stringFacts() << endl <<
              stringRules() << endl <<
              stringQueries() << endl <<
              stringDomain() << endl;
        return ss.str();
    }
};


#endif // DATALOG
