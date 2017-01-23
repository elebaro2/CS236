#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <vector>
#include <cctype>
#include <chrono>

#include "parser.h"
#include "reader.h"
#include "database.h"
//`````typedef std::chrono::high_resolution_clock Clock;
using namespace std;

int main(int agc, char* argv[]){
    string fn = argv[1];
    /*sends file to the reader which creates a list of tokens*/
   Reader reader(fn);
    /*List of tokens*/
    vector<token> tokenVector = reader.getTokens();
   try{/*parses through the tokens to check them against the datalog grammar*/
       Parser p(tokenVector);
        /*valid token sequences are saved in the datalog*/
        datalog d = p.getData();
        Database db(d.getSchemes(), d.getFacts(), d.getQueries(), d.getRules());
        db.runQueries();
    }
    /*Any tokens that invalidate the grammar are thrown
    *and displayed*/
    catch(string t){
        cout << "Failure!\n" <<"  " << t;
    }


    return 0;
};
