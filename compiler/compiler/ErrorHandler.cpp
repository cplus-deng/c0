/*
    ErrorHandler.cpp
    ����������ʵ��
*/
#include "ErrorHandler.h"
#include <iostream>
#include <string>
using namespace std;

#define TOKEN_SYMBOL    (*lexicalAnalyzer.token.symbol)     // string ���͵� token
#define TOKEN_NUMBER    (lexicalAnalyzer.token.number)      // int ���͵� token
#define SYMBOL_TYPE     (lexicalAnalyzer.symbolType)        // ���ŵ� SymbotType ����ֵ
// ����꣬�򻯶Է��ź����ͻ�ȡ    

#define NEXTSYM lexicalAnalyzer.nextSymbol()                // ����꣬�򻯻�ȡ��һ�����ŵĲ���

// ����ģʽ
ErrorHandler& ErrorHandler::initialErrorHandler(LexicalAnalyzer& la, bool warning) {
    static ErrorHandler theErrorHandler(la, warning);
    return theErrorHandler;
}

static map<ErrorType, string> errout;       // ������������������Ӧ��

// ��ʼ�� errout ��
static void initialErrout() {
#define INSERT(type, msg) errout.insert(pair<ErrorType, string>(ErrorType::type, msg))
    // ��ʼ���������ͱ�
    INSERT(NO_PERIOD, "period expected");
    INSERT(ILLEGAL_CHAR, "illegal symbol");
    INSERT(NO_FINISH, "file should be end");
    INSERT(FINISH, "unexpected end of file");
    INSERT(NO_CONST, "const keyword expected");
    INSERT(NO_SEMICOLON, "semilcolon expected");
    INSERT(NO_IDENTIFIER, "identifier expected");
    INSERT(NO_EQUAL, "equality expected");
    INSERT(NO_CHNUM, "number or charactor expected");
    INSERT(NO_SINGLEQUOTE, "single quote expected");
    INSERT(NO_DOUBLEQUOTE, "double quote expected");
    INSERT(ILLEGAL_STRING, "illegal symbol in string");
    INSERT(NO_NUMBER, "number expected");
    INSERT(INT_OVERFLOW, "integer input is too long, it will be cut off");
    INSERT(NO_VAR, "var keyword expected");
    INSERT(NO_COLON, "colon expected");
    INSERT(NO_LEFTSQUARE, "symbol [ expected");
    INSERT(NO_RIGHTSQUARE, "symbol ] expected");
    INSERT(NO_OF, "of keyword expected");
    INSERT(NO_BASICTYPE, "basic type (integer or char) expected");
    INSERT(NO_PROCEDURE, "procedure keyword expected");
    INSERT(NO_FUNCTION, "function keyword expected");
    INSERT(NO_LEFTBRACKET, "symbol ( expected");
    INSERT(NO_RIGHTBRACKET, "symbol ) expected");
    INSERT(CONSTANT, "unexpected constant identifier");
    INSERT(NO_ASSIGN, "symbol = expected. Here is ecpected to be an assigning statement");
    INSERT(NO_VALUE, "value expected");
    INSERT(UNDEFINED, "undefined identifier");
    INSERT(CANNOT_CALL, "identifier is not a name of function or procedure");
    INSERT(INT_TO_CHAR, "assign a integer to a char. Maybe cut off");
    INSERT(TOO_LESS_ARG, "too less real parameter");
    INSERT(TOO_MANY_ARG, "too many real parameter");
    INSERT(UNVARIABLE, "real parameter is not variable");
    INSERT(NO_RELATION, "relation operator expected");
    INSERT(NO_IF, "if keyword expected");
    INSERT(NO_THEN, "then keyword expected");
    INSERT(NO_ELSE, "else keyword expected");
    INSERT(NO_DO, "do keyword expected");
    INSERT(NO_WHILE, "while keyword expected");
    INSERT(NO_FOR, "for keyword expected");
    INSERT(NO_DOWN_TO, "downto or to keyword expected");
    INSERT(NO_BEGIN, "begin keyword expected");
    INSERT(NO_END, "end keyword expected");
    INSERT(NO_READ, "read keyword expected");
    INSERT(NO_WRITE, "write keyword expected");
#undef INSERT
}

// ����
void ErrorHandler::printWarning(ErrorType err) {
    if (warning) {
        warningCount++;
        cerr << "Warn " << warningCount << ": ";
        cerr << "line " << lexicalAnalyzer.lineNumber << ": warning W" << err << ": ";
        cerr << errout.find(err)->second << endl;
    }
}

// ���渽����Ϣ
void ErrorHandler::printWarning(ErrorType err, const string& msg) {
    if (warning) {
        warningCount++;
        cerr << "Warn " << warningCount << ": ";
        cerr << "line " << lexicalAnalyzer.lineNumber << ": warning W" << err << ": ";
        cerr << errout.find(err)->second << ". (" << msg << ")" << endl;
    }
}

// ���캯��
ErrorHandler::ErrorHandler(LexicalAnalyzer& la, bool warning) :
    lexicalAnalyzer(la), warning(warning) {
    errorCount = 0;
    warningCount = 0;
    initialErrout();
}

// ���������Ϣ
void ErrorHandler::printError(ErrorType err) {
    errorCount++;
    cerr << "Err " << errorCount << ": ";
    cerr << "line " << lexicalAnalyzer.lineNumber << ": error E" << err << ": ";
    cerr << errout.find(err)->second << endl;
}

// ���������Ϣ����������Ϣ��
void ErrorHandler::printError(ErrorType err, const string& msg) {
    errorCount++;
    cerr << "Err " << errorCount << ": ";
    cerr << "line " << lexicalAnalyzer.lineNumber << ": error E" << err << ": ";
    cerr << errout.find(err)->second << ". (" << msg << ")" << endl;
}

// ���������ķ���֮һ����������Ӧ����
ErrorEnd ErrorHandler::skip() {
    list<pair<SymbolType, ErrorEnd> >::reverse_iterator it = endmap.rbegin();
    while (it != endmap.rend()) {
        if (it->first == SYMBOL_TYPE)
            break;
        it++;
    }
    while (it == endmap.rend()) {
        NEXTSYM;
        it = endmap.rbegin();
        while (it != endmap.rend()) {
            if (it->first == SYMBOL_TYPE)
                break;
            it++;
        }
    }
    return it->second;
}

// �� endmap ���һ��
void ErrorHandler::addEndmap(SymbolType st, ErrorEnd ee) {
    endmap.push_back(pair<SymbolType, ErrorEnd>(st, ee));
}

// �� endmap ���Ƴ� SymbolType�����û�У��޲���
void ErrorHandler::removeEndmap(SymbolType st) {
    list<pair<SymbolType, ErrorEnd> >::reverse_iterator it = endmap.rbegin();
    while (it != endmap.rend() && it->first != st)
        it++;
    if (it != endmap.rend())
        endmap.remove(*it);
}