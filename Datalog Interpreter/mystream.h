#ifndef MYSTREAM
#define MYSTREAM
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

class MyStream{
public:
    MyStream(string mystr, int linenum){
        mystring = mystr;
        LN = linenum;
        index = 0;
}
    ~MyStream(){}

    int getLN() const{
        return LN;
    }

    /*returns the current char in the string*/

    char getChar(){
        return mystring[index];
    }

    /*iterates the index counter to the next char and returns it*/

    char Next(){
        index++;
        return mystring[index];
    }

    /*checks the next char, but doesn't move the index counter*/

    char soft_next(){
        return mystring[index+1];
    }

    /*checks the char after the next char, but doesn't move the index counter*/

    char soft_next_two(){
        return mystring[index+2];
    }

    string see_mystr() const {
        return mystring;
    }


private:
    string mystring;
    int LN;
    int index;

};
#endif //MYSTREAM
