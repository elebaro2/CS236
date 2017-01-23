#ifndef PARSER_H
#define PARSER_H
#include "datalog.h"
#include "token.h"
#include <vector>


using namespace std;

class Parser{
public:


    Parser(){
        data = datalog();
        tokens = vector<token>();
        index = 0;
        current = token();
    }

    /*parses through the vector of tokens*/

    Parser(vector<token>& tokenvector){
        tokens = tokenvector;
        index = 0;
        current = tokens[0];
        p = Predicate();
        r = Rule();
        parseDatalog();
    }
    ~Parser(){}

    int index;
    token current;
    vector<token> tokens;
    datalog data;
    Predicate p;
    Rule r;

    /*determines whether the token is a valid token based on the token type defined*/
    bool match(string tokenType){
//        cout << "HERE\n" << " " << current.getType() << " " << tokenType << endl;
        if(tokenType == current.getType()){
            index++;
            current = tokens[index];
            return true;
        }
        else{
            throw string(current.printTokens());
        return false;
        }
    }
    /*Parses through the tokens saved and determines
    *whether they are valid
    *valid tokens are put into the vector that corresponds to their type in the
    *datalog.h file*/
    void parseDatalog(){
        match("SCHEMES");
        match("COLON");
        parseScheme();
        parseSchemeList();
        match("FACTS");
        match("COLON");
        parseFactList();
        match("RULES");
        match("COLON");
        parseRuleList();
        match("QUERIES");
        match("COLON");
        parseQuery();
        parseQueryList();
    }
    /*schemeList-> scheme schemeList | lambda*/
    void parseSchemeList(){
        if(current.getType() == "ID"){
            parseScheme();
        }
        if(current.getType() == "ID")
            parseSchemeList();
    }
    /*scheme -> ID LEFT_PAREN ID idList RIGHT_PAREN*/
    void parseScheme(){
        p = Predicate(current.getValue());
        match("ID");
        match("LEFT_PAREN");
        if(current.getType() == "ID"){
            p.addParameter(Parameter(current.getType(), current.getValue()));
        }
        match("ID");
        if(current.getType() == "COMMA"){
            parseIdList();
        }
        match("RIGHT_PAREN");
        data.addScheme(p);
    }
    /*idList -> COMMA ID idList | lambda*/
    void parseIdList(){
        match("COMMA");
        if(current.getType() == "ID"){
            p.addParameter(Parameter(current.getType(), current.getValue()));
        }
        match("ID");
        if(current.getType() == "COMMA")
            parseIdList();
    }
    /*factList -> fact factList | lambda*/
    void parseFactList(){
        if(current.getType() == "ID"){
            parseFact();
            if(current.getType() == "ID"){
                parseFactList();
            }
        }

    }
    /*fact -> ID LEFT_PAREN STRING stringList RIGHT_PAREN PERIOD*/
    void parseFact(){
        p = Predicate(current.getValue());
        match("ID");
        match("LEFT_PAREN");
        if(current.getType() == "STRING"){
            p.addParameter(Parameter(current.getType(),current.getValue()));
        }
        match("STRING");
        if(current.getType() == "COMMA"){
            parseStringList();
        }
        match("RIGHT_PAREN");
        match("PERIOD");
        data.addFact(p);
    }
    /*ruleList -> rule ruleList | lambda*/
    void parseRuleList(){
        if(current.getType() == "ID"){
            parseRule();
            if(current.getType() == "ID"){
                parseRuleList();
            }
        }
    }
    /*rule -> headPredicate COLON_DASH Predicate predicateList PERIOD*/
    void parseRule(){
        parseHeadPredicate();
        r = Rule(p);
        match("COLON_DASH");
        parsePredicate();
        if(current.getType() == "COMMA"){
            parsePredicateList();
        }
        match("PERIOD");
        data.addRule(r);
    }
    /*queryList -> query queryList | lambda*/
    void parseQueryList(){
        if(current.getType() == "EOF"){
            return;
        }
        else{
            parseQuery();
            if(current.getType() == "ID")
                parseQueryList();

        }

    }
    /* stringList -> COMMA STRING stringList | lambda*/
    void parseStringList(){
        match("COMMA");
        if(current.getType() == "STRING"){
            p.addParameter(Parameter(current.getType(), current.getValue()));
        }
        match("STRING");
        if(current.getType() == "COMMA")
            parseStringList();
    }
    /*headPredicate -> ID LEFT_PAREN ID idList RIGHT_PAREN*/
    void parseHeadPredicate(){
        p = Predicate(current.getValue());
        match("ID");
        match("LEFT_PAREN");
        if(current.getType() == "ID"){
            p.addParameter(Parameter(current.getType(), current.getValue()));
        }
        match("ID");
        if(current.getType() == "COMMA"){
            parseIdList();
        }
        match("RIGHT_PAREN");
    }
    /*predicate -> ID LEFT_PAREN parameter parameterList RIGHT_PAREN*/
    void parsePredicate(){
        p = Predicate(current.getValue());
        match("ID");
        match("LEFT_PAREN");
        parseParameter();
        if(current.getType() == "COMMA"){
            parseParameterList();
        }
        match("RIGHT_PAREN");
        r.addPredicate(p);
    }
    /*predicateList -> COMMA predicate predicateList | lambda*/
    void parsePredicateList(){
        match("COMMA");
        parsePredicate();
        if(current.getType() == "COMMA")
            parsePredicateList();
    }
    /*parameter -> STRING | ID | expression*/
    void parseParameter(){
        if(current.getType() == "STRING" || current.getType() == "ID"){
            p.addParameter(Parameter(current.getType(), current.getValue()));
            match(current.getType());
        }/*checks for expression*/
        if(current.getType() == "LEFT_PAREN"){
            parseExpression();
        }
    }
    /*parameterList -> COMMA parameter parameterList | lambda*/
    void parseParameterList(){
        match("COMMA");
        parseParameter();
        if(current.getType() == "COMMA"){
           parseParameterList();
        }
    }
    /*expression -> LEFT_PAREN parameter operator parameter RIGHT_PAREN*/
    void parseExpression(){
        match("LEFT_PAREN");
        parseParameter();
        parseOperator();
        parseParameter();
        match("RIGHT_PAREN");
    }
    /*operator -> ADD | MULTIPLY*/
    void parseOperator(){
        if(current.getType() == "ADD")
            match("ADD");
        if(current.getType() == "MULTIPLY")
            match("MULTIPLY");
    }
    /*query -> predicate Q_MARK*/
    void parseQuery(){
        p = Predicate(current.getValue());
        parsePredicate();
        match("Q_MARK");
        data.addQuery(p);
    }

    datalog getData()const{
        return data;
    }
};

#endif // PARSER_H

/*datalogProgram	->	SCHEMES COLON scheme schemeList
                        FACTS COLON factList
                        RULES COLON ruleList
                        QUERIES COLON query queryList


scheme   	-> 	ID LEFT_PAREN ID idList RIGHT_PAREN


schemeList	->	scheme schemeList | lambda


idList  	->	COMMA ID idList | lambda


fact    	-> 	ID LEFT_PAREN STRING stringList RIGHT_PAREN PERIOD


factList	->	fact factList | lambda


rule    	->	headPredicate COLON_DASH predicate predicateList PERIOD


ruleList	->	rule ruleList | lambda


headPredicate	->	ID LEFT_PAREN ID idList RIGHT_PAREN


predicate	->	ID LEFT_PAREN parameter parameterList RIGHT_PAREN


predicateList	->	COMMA predicate predicateList | lambda


parameter	->	STRING | ID | expression


parameterList	-> 	COMMA parameter parameterList | lambda


expression	-> 	LEFT_PAREN parameter operator parameter RIGHT_PAREN


operator	->	ADD | MULTIPLY


query	        ->      predicate Q_MARK


queryList	->	query queryList | lambda


stringList	->	COMMA STRING stringList | lambda */
