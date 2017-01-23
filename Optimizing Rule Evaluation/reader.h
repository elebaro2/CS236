#ifndef READER
#define READER
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cctype>
#include <cstdio>

#include "token.h"
#include "mystream.h"

using namespace std;


class Reader
{
public:
    Reader(string ip){
        /*Recieves filename and starts at line 1*/
        count = 1;
        read_file(ip);
    }
    ~Reader(){}


    vector<token> getTokens() const{
        return TOKS;
    }
private:
    /*turns the char into type string*/
    string to_string(char x){
        stringstream ss;
        ss << x;
        return ss.str();
    }
    //opens and reads the file so that it can pull out the contents
    void read_file(string ip){
        LINE_NUM = total_lines(ip);
        ifstream fin;
        fin.open(ip.data());
        if(!fin.eof()){
            char nextChar;
            string s = "";
            while(fin.get(nextChar)){
                s+=nextChar;
            }
            s+='\n';
            read_stream(MyStream(s,1));
        }
        else
            cout <<"Cannot open file\n";
    }

    /*keeps track of what line is currently being read*/

    int count;

    /*iterates through the file, looking for whitespaces and words, everything else is sent to
     *be tried against non_word tokens, including strings and comments. Outputs EOF token when there
     *are no more chars to read*/

    void read_stream(MyStream ss){
        while(count < LINE_NUM+1){
            char current = ss.getChar();
            if(current == '\n'){
                count++;
            }
            if(isspace(current)){
                current = ss.Next();
            }
            else if(isalpha(current)){
                ss = read_word(ss);
                current = ss.Next();
            }
            else{
                ss = add_punc(ss);
                current = ss.Next();
            }
        }
        if(count == LINE_NUM +1 ){
            TOKS.push_back(token("EOF", "", count - 1));
        }
    }
    /*reads the word until it reaches whitespace or a
     * non alphanumeric char and then sends the word to
     * check its type*/

    MyStream read_word(MyStream& ss){
        char current = ss.getChar();
        string str;
        str += current;
        while(isalnum(ss.soft_next())){
            if(current == '\n')
                break;
            current = ss.Next();
            str += current;
        }
        add_word(str, count);
        return ss;
    }

    /*assigns correct type designation and creates the word token
     *and adds the token to the tokens vector*/

    void add_word(string str, int linenum){
        string type;

        if(str == "Schemes")
            type = "SCHEMES";
        else if(str == "Facts")
            type = "FACTS";
        else if(str == "Rules")
            type = "RULES";
        else if(str == "Queries")
            type = "QUERIES";
        else
            type = "ID";

        TOKS.push_back(token(type, str, linenum));
    }

    bool isUndef(MyStream& ss){
        string str = "";
        while((ss.Next() != '\'')){
            str+=ss.getChar();
        }
        if(ss.soft_next() == '\''){
            return true;
        }
        return false;
    }

    bool isGood(char x){
        if(x == '\'')
            return false;
        if(isspace(x))
            return true;
        if(isalpha(x))
            return true;
        return false;
    }


    /*Assigns the correct type to char that doesn't fit in the alphanumeric section
     *Is split into two to devide single char types and types that check for more than one char*/
    MyStream add_punc(MyStream& ss){
        switch(ss.getChar()){
        case ',':{
            TOKS.push_back(token("COMMA", ",", count));
        }break;
        case '.':{
            TOKS.push_back(token("PERIOD", ".", count));
        }break;
        case '?':{
            TOKS.push_back(token("Q_MARK", "?", count));
        }break;
        case '(':{
            TOKS.push_back(token("LEFT_PAREN", "(", count));
        }break;
        case ')':{
            TOKS.push_back(token("RIGHT_PAREN", ")", count));
        }break;
        case '*':{
            TOKS.push_back(token("MULTIPLY", "*", count));
        }break;
        case '+':{
            TOKS.push_back(token("ADD", "+", count));
        }break;
        default:{
            return add_nword_two(ss);
            }
        }
        return ss;
    }

    /*Checks whether there is a valid string token*/

    MyStream str_tok(MyStream& ss){
        int line = count;
        string str = "";
        char current = ss.getChar();
        str += ss.getChar();
        int i = 1;
        while(count != LINE_NUM+1){
            while(((ss.soft_next()) != '\'') && (count != LINE_NUM+1)){
                current = ss.Next();
                str+=current;
                if(current == '\n')
                    count++;
          }
            if(ss.soft_next() == '\''){
                i++;
                current = ss.Next();
                str+=current;
            }
            if(is_str(i, ss)){
                TOKS.push_back(token("STRING", str, line));
                return ss;
          }

        }
     str.pop_back();
     TOKS.push_back(token("UNDEFINED", str, line));
     return ss;
    }
    /*bool that checks if the end of the string has been reached*/

    bool is_str(int i, MyStream& ss){
        if(iseven(i) && (ss.soft_next() != '\''))
            return true;
        return false;
    }

    /*Checks whether there is a valid Comment token*/

    MyStream comm_tok(MyStream& ss){
        int line = count;
        string str = "";
        char current = ss.getChar();
        str+=current;

        if(ss.soft_next() != '|'){
            return one_comm(ss, line);
        }
        else if(ss.soft_next() == '|'){
            while(count!= LINE_NUM+1){
                current = ss.Next();
                str+= current;
                if(current == '\n')
                    count++;
            if((is_mul_comm(ss.soft_next(), ss.soft_next_two()))){
                current = ss.Next();
                str += current;
                    current = ss.Next();
                    str += current;
                    //TOKS.push_back(token("COMMENT", str, line));
                    return ss;

                }
            }
        }
        str.pop_back();
        TOKS.push_back(token("UNDEFINED", str, line));
        return ss;
    }


    /*bool to check if there are more chars in a block comment before it checks for the
     *end of the block comment*/

    bool keep_read_comm(char x){
        if((x != '|') && (count != LINE_NUM+1))
          return true;
        return false;
    }

    /*checks if the multiline block comment has a valid ending*/

    bool is_mul_comm(char x, char y){
        if((x == '|') && (y == '#'))
            return true;
        return false;
    }

    /*creates single line comment*/

    MyStream one_comm(MyStream& ss, int line){
        char current = ss.getChar();
        string str;
        str += current;
        while(ss.soft_next() != '\n'){
            current = ss.Next();
            str += current;
        }
        //TOKS.push_back(token("COMMENT", str, line));
        current = ss.Next();
        count++;
        return ss;
    }

    /*checks for multi char tokens, any that don't match are invalid.
     *immediately returns valid multi char tokens*/

    MyStream add_nword_two(MyStream& ss){
        string in = to_string(ss.getChar());
        string str = "";
        switch(ss.getChar()){
        case '\'':{
            return str_tok(ss);
        }break;
        case':':{
            if(ss.soft_next() == '-'){
                TOKS.push_back(token("COLON_DASH", ":-", count));
                ss.Next();
            }
            else
                TOKS.push_back(token("COLON", ":", count));
        }break;
        case'#':{
            return comm_tok(ss);
        }break;
       default:{
            TOKS.push_back(token("UNDEFINED", in, count));
        }
        }
            return ss;
    }

    /*used to check for even ammount of char '\'' */

    bool iseven(int n){
        if(n%2 == 0)
            return true;
        return false;
    }

    /*counts the number of lines in the file*/

    int total_lines(string ip){
        char c;
        int num = 0;
        ifstream fin;
        fin.open(ip.c_str());
        while(fin.good()){
           c = fin.get();
            if(c == '\n')
                num++;
        }
        fin.close();
        return num + 1;
    }

    /*vector of tokens*/

    vector<token> TOKS;

    /*number of lines in the file*/

    int LINE_NUM;

};
#endif //READER
