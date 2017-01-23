#ifndef node_h
#define node_h
#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include <set>
using namespace std;
class Node : public vector<string> {
    friend class Graph;

private:
    int pon;
    //post order number
    bool visited;
    //If the node was already visited in the graph.
    set<int> dependencies;
    //The node numbers this node is dependant on.

public:
    Node(){
        pon = -1;
        visited = false;
    }

    ~Node(){
        dependencies.clear();
    }

    //Setters
    void was_visited(){
        visited = true;
    }

    void assign_pon(int num){
        pon = num;
    }

    void insert_adjacent_nodes(int rule_index){
        dependencies.insert(rule_index);
    }

    string print_adjacent_nodes(){
        stringstream ss;
        for(auto adjacent_node:dependencies)
            ss << " R"<< adjacent_node;
        return ss.str();
    }

    set<int> get_adjacent_nodes(){
        return dependencies;
    }
};
#endif
