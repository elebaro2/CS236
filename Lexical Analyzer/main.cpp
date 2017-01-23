#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <vector>
#include <cctype>

#include "reader.h"

using namespace std;

int main(int agc, char* argv[]){


    string fn = argv[1];
    /*sends file to the reader*/
    Reader reader(fn);

    stringstream os;

vector<token> tokenVector = reader.getTokens();

    for(unsigned int i = 0; i < tokenVector.size(); i++){
        os << tokenVector[i].printTokens();
    }
    cout << os.str();
    cout << "Total Tokens = " << tokenVector.size() << endl;

    return 0;
}
