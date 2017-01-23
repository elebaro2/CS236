#ifndef RULE_H
#define RULE_H
#include "predicate.h"
#include <vector>

using namespace std;
/*contains the head predicate, colon dash, a list of predicates and the period at the end*/
class Rule{
public:
    Rule(){
        predicate = Predicate();
        predicateList = vector<Predicate>();
    }

    Rule(Predicate p){
        predicate = p;
        predicateList = vector<Predicate> ();
    }

    Rule (Predicate pred, vector<Predicate>& predList){
        predicate = pred;
        predicateList = predList;
    }

    Predicate predicate;
    vector<Predicate> predicateList;

    void addPredicate (Predicate pred){
        predicateList.push_back(pred);
    }

    vector<Predicate> getPredicateList()const{
        return predicateList;
    }

    Predicate getPredicate(){
        stringstream ss;
        ss << predicate.toString() << " :- ";
        for(size_t i = 0; i < predicateList.size(); i++){
            if(i == 0)
                ss << predicateList[i].toString();
            else
                ss << "," << predicateList[i].toString();
        }
        ss << ".";
        return ss.str();
    }

    string toString(){

        stringstream ss;
        ss << predicate.toString() << " :- ";
        for(size_t i = 0; i < predicateList.size(); i++){
            if(i == 0)
                ss << predicateList[i].toString();
            else
                ss << "," << predicateList[i].toString();
        }
        ss << ".";
        return ss.str();
    }
};

#endif // RULE_H
