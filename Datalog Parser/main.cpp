#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <vector>
#include <cctype>

#include "parser.h"
#include "reader.h"

using namespace std;

int main(int agc, char* argv[]){


    string fn = argv[1];
    /*sends file to the reader which creates a list of tokens*/
    Reader reader(fn);

    stringstream os;
    /*List of tokens*/
    vector<token> tokenVector = reader.getTokens();
/*    cout << "vector of tokens\n";
    for(unsigned int i = 0; i < tokenVector.size(); i++)
        cout << tokenVector[i].printTokens() << endl;*/
   try{/*parses through the tokens to check them against the datalog grammar*/
        Parser p(tokenVector);
        /*valid token sequences are saved in the datalog*/
        datalog d = p.getData();
        os << d.toString();
        cout << os.str();
    }
    /*Any tokens that invalidate the grammar are thrown
    *and displayed*/
    catch(string t){
        cout << "Failure!\n" <<"  " << t;
}

    return 0;
};
