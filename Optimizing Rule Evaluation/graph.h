#ifndef GRAPH
#define GRAPH
#include <stdio.h>
#include <iostream>
#include <string>
#include <map>
#include "node.h"
#include "rule.h"

class Graph{
    friend class Database;
private:
    int postorder;
    vector<Rule> rules;
    map<int, Node> graph;
    map<int, Node> reverse_graph;
    map<int, set<int>> scc;

public:
    Graph(vector<Rule>& rules){
        postorder = 1;
        this->rules = rules;

        this->make_graphs();
        this->dfsForest();
        this->find_scc();
    }
    ~Graph(){}

    void add_dependencies(int rule_index){
        Rule dependency_rule = rules[rule_index];
        int rule_dependent_index = 0;
        for(auto depend_rule :rules){
            for(auto pred : depend_rule.predicateList){
                if(dependency_rule.predicate.getID() == pred.getID()){
                    graph[rule_dependent_index].insert_adjacent_nodes(rule_index);
                    reverse_graph[rule_index].insert_adjacent_nodes(rule_dependent_index);
                }
            }
            rule_dependent_index++;
        }
    }

    void make_graphs(){
        for(size_t i = 0; i < rules.size(); i++){
            Node forward;
            Node reverse;
            graph.insert(pair<int, Node> (i, forward));
            reverse_graph.insert(pair<int, Node> (i, reverse));
        }

        int rule_index = 0;
        for(auto rule: rules){
            add_dependencies(rule_index);
            rule_index++;
        }
    }

    void dfs(int node_index){
        Node& n = reverse_graph[node_index];
        n.was_visited();
        set<int> adjacent_nodes = n.get_adjacent_nodes();

        for(auto set_index:adjacent_nodes){
            if(reverse_graph[set_index].visited == false){
                dfs(set_index);
            }
        }

        n.assign_pon(postorder);
        postorder++;
    }

    void dfsForest(){
        for(auto node: reverse_graph){
            if(node.second.visited == false)
                dfs(node.first);
        }
    }

    vector<int> getHighestToLowestPON(){
        vector<int> highestToLowest;

        for(auto i = postorder; i > 0; i--){
            for(auto reverse: reverse_graph){
                if(i == reverse.second.pon)
                    highestToLowest.push_back(reverse.first);
            }
        }
        return highestToLowest;
    }

    void scc_in_dfs(int index, set<int>& scc){
        Node& n = graph[index];
        n.was_visited();
        set<int> adjacent_nodes = n.get_adjacent_nodes();
        for(auto set_index : adjacent_nodes){
            if(graph[set_index].visited == false)
                scc_in_dfs(set_index, scc);
        }
        scc.insert(index);
    }

    void find_scc(){
        vector<int>  hightToLowestRules = getHighestToLowestPON();
        for(size_t i = 0; i < hightToLowestRules.size(); i++){
            set<int>scc_set;
            if(graph[hightToLowestRules[i]].visited == false)
                scc_in_dfs(hightToLowestRules[i], scc_set);
            scc.insert(pair<int, set<int>> (hightToLowestRules[i], scc_set));
        }
    }


    string print_graph(){
        stringstream ss;
        ss << "Dependency Graph\n";
        for(auto forward:graph){
            ss << "R" << forward.first << ":" << forward.second.print_adjacent_nodes() << endl;
        }
        ss << endl;
        return ss.str();
    }

    string print_reverse_graph(){
        stringstream ss;
        ss<<"Reverse Graph\n";
        for(auto reverse:reverse_graph){
            ss << "\n  R" << reverse.first << ":" << reverse.second.print_adjacent_nodes();
        }
        ss << "\n\n";
        return ss.str();
    }


    string print_scc(){
        stringstream ss;
        ss<<"Postorder Numbers";
        for(auto forward:reverse_graph){
            ss << "\n  R" << forward.first<< ":" << forward.second.pon;
        }

        ss<< "\n\nSCC Search Order";
        vector<int> search_order = getHighestToLowestPON();
        for(auto search : search_order)
            ss << "\n  R" << search;
        ss << "\n\n";
        return ss.str();

    }
};

#endif // GRAPH

