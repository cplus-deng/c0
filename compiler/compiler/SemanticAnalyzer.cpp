/*
    SemanticAnalyzer.cpp
    语义分析器的实现
*/
#include "SemanticAnalyzer.h"

SemanticAnalyzer& SemanticAnalyzer::initialSemanticAnalyzer(
    ErrorHandler& eh, SymbolTableManager& stm, Quadruples& q, LexicalAnalyzer& la) {
    static SemanticAnalyzer theSemanticAnalyzer(eh, stm, q, la);
    return theSemanticAnalyzer;
}

SemanticAnalyzer::SemanticAnalyzer(ErrorHandler& eh, SymbolTableManager& stm,
    Quadruples& q, LexicalAnalyzer& la) :
    errorHandler(eh), table(stm), quadruples(q), lexicalAnalyzer(la) {
    temporaryCount = 0;
}

#define ADDMAP(st, ee)  errorHandler.addEndmap(SymbolType::st, ErrorEnd::ee)
#define DELMAP(st)      errorHandler.removeEndmap(SymbolType::st)

#define TOKEN_SYMBOL    (*lexicalAnalyzer.token.symbol)     // string 类型的 token
#define TOKEN_NUMBER    (lexicalAnalyzer.token.number)      // int 类型的 token
#define SYMBOL_TYPE     (lexicalAnalyzer.symbolType)        // 符号的 SymbotType 类型值
// 定义宏，简化对符号和类型获取    

#define NEXTSYM lexicalAnalyzer.nextSymbol()

#define ERROUT  errorHandler.printError
#define WARNOUT errorHandler.printWarning

string SemanticAnalyzer::newTemporary() {
    temporaryCount++;
    return stringPlusInt("t_", temporaryCount);
}

void SemanticAnalyzer::programEndmapInit() {
    ADDMAP(FINISH, PROGRAM);
}

void SemanticAnalyzer::blockEndmapInit() {
    ADDMAP(SEMICOLON, BLOCK);
}

void SemanticAnalyzer::blockEndmapClear() {
    DELMAP(SEMICOLON);
}

void SemanticAnalyzer::constDeclareEndmapInit() {
    ADDMAP(CONST_KW, CONST_DECLARE);
    ADDMAP(LET_KW, CONST_DECLARE);
    ADDMAP(FN_KW, CONST_DECLARE);
    ADDMAP(SEMICOLON, CONST_DECLARE);
}

void SemanticAnalyzer::constDeclareSkip() {
    if (SYMBOL_TYPE == SymbolType::SEMICOLON)
        NEXTSYM;
}

void SemanticAnalyzer::constDeclareEndmapClear() {
    DELMAP(CONST_KW);
    DELMAP(LET_KW);
    DELMAP(FN_KW);
    DELMAP(SEMICOLON);
}

void SemanticAnalyzer::constDefineEndmapInit() {
    ADDMAP(COMMA, CONST_DEFINE);
}

void SemanticAnalyzer::constDefineEndmapClear() {
    DELMAP(COMMA);
}

void SemanticAnalyzer::constDefine(const string& id, BasicType type, int value) {
    table.ste = new STE;
    table.ste->name = id;
    table.ste->type = IdentifierType::CONST;
    table.ste->valueType = type;
    table.ste->multiInt = value;
    table.insert();
}

void SemanticAnalyzer::charEndmapInit() {
    ADDMAP(SINGLE_QUOTE, CHAR);
}

void SemanticAnalyzer::charSkip() {
    if (SYMBOL_TYPE == SymbolType::SINGLE_QUOTE)
        NEXTSYM;
}

void SemanticAnalyzer::charEndmapClear() {
    DELMAP(SINGLE_QUOTE);
}

void SemanticAnalyzer::stringEndmapInit() {
    ADDMAP(DOUBLE_QUOTE, STRING);
}

void SemanticAnalyzer::stringEndmapClear() {
    DELMAP(DOUBLE_QUOTE);
}

void SemanticAnalyzer::stringSkip() {
    if (SYMBOL_TYPE == SymbolType::DOUBLE_QUOTE)
        NEXTSYM;
}

string& SemanticAnalyzer::stringDefine(const string& str) {
    static int count = 0;
    static string name;
    name = stringPlusInt("string_", ++count);
    table.addString(name, str);
    return name;
}

void SemanticAnalyzer::varDeclareEndmapInit() {
    ADDMAP(FN_KW, VAR_DECLARE);
    ADDMAP(LET_KW, VAR_DECLARE);
    ADDMAP(CONST_KW, VAR_DECLARE);
    ADDMAP(SEMICOLON, VAR_DEFINE);
}

void SemanticAnalyzer::varDeclareEndmapClear() {
    DELMAP(FN_KW);
    DELMAP(LET_KW);
    DELMAP(CONST_KW);
    DELMAP(SEMICOLON);
}

void SemanticAnalyzer::varDefineEndmapInit() {
    ADDMAP(SEMICOLON, VAR_DEFINE);
}

void SemanticAnalyzer::varDefineEndmapClear() {
    DELMAP(SEMICOLON);
}

void SemanticAnalyzer::varDefine(const string& name, BasicType bType) {
        table.ste = new STE;
        table.ste->name = name;
        table.ste->type = IdentifierType::VAR;
        table.ste->valueType = bType;
        table.insert();
}

void SemanticAnalyzer::functionDeclareEndmapInit() {
    ADDMAP(R_BRACE, FUNCTION_DECLARE);
    ADDMAP(FN_KW, FUNCTION_DECLARE);
    ADDMAP(CONST_KW, FUNCTION_DECLARE);
    ADDMAP(LET_KW, FUNCTION_DECLARE);
}

void SemanticAnalyzer::functionDeclareEndmapClear() {
    DELMAP(R_BRACE);
    DELMAP(FN_KW);
    DELMAP(CONST_KW);
    DELMAP(LET_KW);
}

void SemanticAnalyzer::functionDeclareSkip() {
    if (SYMBOL_TYPE == SymbolType::R_BRACE)
        NEXTSYM;
}

void SemanticAnalyzer::functionDefine(const string& funcname,
    vector<Argument>& args, BasicType type) {
    table.ste = new STE;
    table.ste->name = funcname;
    table.ste->type = IdentifierType::FUNCTION;
    table.ste->valueType = type;
    table.insert();
    if (!args.empty())
        table.defineArguments(table.ste, args);         // 处理参数表
}

void SemanticAnalyzer::formalParameterEndmapInit() {
    ADDMAP(SEMICOLON, FORMAL_PARAMETER);
    ADDMAP(R_PAREN, FORMAL_PARAMETER);
}

void SemanticAnalyzer::formalParameterEndmapClear() {
    DELMAP(SEMICOLON);
    DELMAP(R_PAREN);
}

void SemanticAnalyzer::formalParameterAdd(vector<string>& names, BasicType type,
    bool vary, vector<Argument>& args) {
    for (vector<string>::iterator it = names.begin(); it != names.end(); it++)
        args.push_back(Argument(*it, type, vary));
}

void SemanticAnalyzer::statementEndmapInit() {
    ADDMAP(SEMICOLON, STATEMENT);
    ADDMAP(BEGIN, STATEMENT);
    ADDMAP(FOR, STATEMENT);
    ADDMAP(DO, STATEMENT);
    ADDMAP(IF, STATEMENT);
    ADDMAP(READ, STATEMENT);
    ADDMAP(WRITE, STATEMENT);
}

void SemanticAnalyzer::statementEndmapClear() {
    DELMAP(SEMICOLON);
    DELMAP(BEGIN);
    DELMAP(FOR);
    DELMAP(DO);
    DELMAP(IF);
    DELMAP(READ);
    DELMAP(WRITE);
}

void SemanticAnalyzer::negative(const string& oper1, string& dest) {
    dest = newTemporary();
    quadruples.addTail(new Quadruple(Operator::NEG, oper1, "", dest));
}

void SemanticAnalyzer::addition(const string& oper1, const string& oper2, string& dest) {
    dest = newTemporary();
    quadruples.addTail(new Quadruple(Operator::ADD, oper1, oper2, dest));
}

void SemanticAnalyzer::subtraction(const string& oper1, const string& oper2, string& dest) {
    dest = newTemporary();
    quadruples.addTail(new Quadruple(Operator::SUB, oper1, oper2, dest));
}

void SemanticAnalyzer::multiplicatioin(const string& oper1, const string& oper2, string& dest) {
    dest = newTemporary();
    quadruples.addTail(new Quadruple(Operator::MUL, oper1, oper2, dest));
}

void SemanticAnalyzer::division(const string& oper1, const string& oper2, string& dest) {
    dest = newTemporary();
    quadruples.addTail(new Quadruple(Operator::DIV, oper1, oper2, dest));
}

void SemanticAnalyzer::termEndmapInit() {
    ADDMAP(PLUS, TERM);
    ADDMAP(MINUS, TERM);
}

void SemanticAnalyzer::termEndmapClear() {
    DELMAP(PLUS);
    DELMAP(MINUS);
}

void SemanticAnalyzer::factorEndmapInit() {
    ADDMAP(MUL, FACTOR);
    ADDMAP(DIV, FACTOR);
}

void SemanticAnalyzer::factorEndmapClear() {
    DELMAP(MUL);
    DELMAP(DIV);
}

void SemanticAnalyzer::intToString(int n, string& str) {
    str = stringPlusInt("n_", n);
}

void SemanticAnalyzer::callFunction(const string& name, string& value) {
    value = newTemporary();
    quadruples.addTail(new Quadruple(Operator::CALF, name, "", value));
}

void SemanticAnalyzer::callProcedure(const string& name) {
    quadruples.addTail(new Quadruple(Operator::CALP, name, "", ""));
}

void SemanticAnalyzer::cut(BasicType desttype, BasicType valuetype, string& value) {
    if (desttype == BasicType::CHAR && valuetype == BasicType::INTEGER) {
        WARNOUT(ErrorType::INT_TO_CHAR);
        string newValue = newTemporary();
        quadruples.addTail(new Quadruple(Operator::CUT, value, "", newValue));
        value = newValue;
    }
}

void SemanticAnalyzer::functionAssign(const string& name, const string& value,
    BasicType functype, BasicType valuetype) {
    if (functype == BasicType::CHAR && valuetype == BasicType::INTEGER) {
        string t = newTemporary();
        assign(t, value, functype, valuetype);
        quadruples.addTail(new Quadruple(Operator::FAS, t, "", name));
    }
    else
        quadruples.addTail(new Quadruple(Operator::FAS, value, "", name));
}

void SemanticAnalyzer::label(const string& lab) {
    quadruples.addTail(new Quadruple(Operator::LAB, lab, "", ""));
}

void SemanticAnalyzer::funcprocStart() {
    label(table.encodePosition());
}

void SemanticAnalyzer::funcprocReturn() {
    quadruples.addTail(new Quadruple(Operator::RTN, "", "", ""));
}

void SemanticAnalyzer::push(const string& value) {
    quadruples.addTail(new Quadruple(Operator::PUSH, value, "", ""));
}

void SemanticAnalyzer::passParameter(const string& value,
    BasicType formaltype, BasicType realtype) {
    if (formaltype == BasicType::CHAR && realtype == BasicType::INTEGER) {
        string t = newTemporary();
        assign(t, value, formaltype, realtype);
        push(t);
    }
    else
        push(value);
}

void SemanticAnalyzer::passAddress(const string& addr,
    BasicType formaltype, BasicType realtype) {
    if (formaltype == BasicType::CHAR && realtype == BasicType::INTEGER)
        errorHandler.printWarning(ErrorType::INT_TO_CHAR, "Integer address to char address");
    push(addr);
}

void SemanticAnalyzer::compare(const string& value1, const string& value2) {
    quadruples.addTail(new Quadruple(Operator::CMP, value1, value2, ""));
}


void SemanticAnalyzer::dissatisfyJump(SymbolType op, const string& lab) {
    Operator jmpop;
    switch (op) {
    case SymbolType::LT:
        jmpop = Operator::JGE;
        break;
    case SymbolType::LE:
        jmpop = Operator::JG;
        break;
    case SymbolType::GT:
        jmpop = Operator::JLE;
        break;
    case SymbolType::GE:
        jmpop = Operator::JL;
        break;
    case SymbolType::EQ:
        jmpop = Operator::JNE;
        break;
    case SymbolType::NEQ:
        jmpop = Operator::JE;
        break;
    default:
        ERROUT(ErrorType::NO_RELATION, "\a");
        exit(-2441);
        break;
    }
    quadruples.addTail(new Quadruple(jmpop, "", "", lab));
}

void SemanticAnalyzer::jumpAnyway(string lab) {
    quadruples.addTail(new Quadruple(Operator::JMP, "", "", lab));
}

string& SemanticAnalyzer::newLabel() {
    return newLabel("");
}

string& SemanticAnalyzer::newLabel(const string& forehead) {
    static string newlab;
    labelCount++;
    newlab = forehead;
    newlab += stringPlusInt("_label_", labelCount);
    return newlab;
}

void SemanticAnalyzer::satisfyJump(SymbolType op, const string& lab) {
    Operator jmpop;
    switch (op) {
    case SymbolType::LESS:
        jmpop = Operator::JL;
        break;
    case SymbolType::LESS_EQUAL:
        jmpop = Operator::JLE;
        break;
    case SymbolType::GREATER:
        jmpop = Operator::JG;
        break;
    case SymbolType::GREATER_EQUAL:
        jmpop = Operator::JGE;
        break;
    case SymbolType::EQUAL:
        jmpop = Operator::JE;
        break;
    case SymbolType::INEQUAL:
        jmpop = Operator::JNE;
        break;
    default:
        ERROUT(ErrorType::NO_RELATION, "\a");
        exit(-2441);
        break;
    }
    quadruples.addTail(new Quadruple(jmpop, "", "", lab));
}

void SemanticAnalyzer::readChar(const string& name) {
    quadruples.addTail(new Quadruple(Operator::RC, "", "", name));
}

void SemanticAnalyzer::readInteger(const string& name) {
    quadruples.addTail(new Quadruple(Operator::RI, "", "", name));
}

void SemanticAnalyzer::writeString(const string& name) {
    quadruples.addTail(new Quadruple(Operator::WS, name, "", ""));
}

void SemanticAnalyzer::writeInteger(const string& value) {
    quadruples.addTail(new Quadruple(Operator::WI, value, "", ""));
}

void SemanticAnalyzer::writeChar(const string& ch) {
    quadruples.addTail(new Quadruple(Operator::WC, ch, "", ""));
}

void SemanticAnalyzer::newline() {
    quadruples.addTail(new Quadruple(Operator::NL, "", "", ""));
}

void SemanticAnalyzer::getAddress(const string& base, const string& offset, string& addr) {
    addr = newTemporary();
    quadruples.addTail(new Quadruple(Operator::GA, base, offset, addr));
}

void SemanticAnalyzer::getContent(const string& base, string& value) {
    value = newTemporary();
    quadruples.addTail(new Quadruple(Operator::ASFA, base, "", value));
}

void SemanticAnalyzer::assignFromAddress(const string& addr, const string& value,
    BasicType addrType, BasicType valueType) {
    quadruples.addTail(new Quadruple(Operator::ASFA, addr, "", value));
    string variable = value;
    cut(valueType, addrType, variable);
}

void SemanticAnalyzer::assignToAddress(const string& addr, const string& value,
    BasicType addrType, BasicType valueType) {
    string variable = value;
    cut(addrType, valueType, variable);
    quadruples.addTail(new Quadruple(Operator::ASTA, variable, "", addr));
}

void SemanticAnalyzer::assignToArray(const string& arrname,
    const string& offset, const string& value,
    BasicType arraytype, BasicType valuetype) {
    string variable = value;
    cut(arraytype, valuetype, variable);
    string tmp = newTemporary();
    quadruples.addTail(new Quadruple(Operator::GA, arrname, offset, tmp));
    quadruples.addTail(new Quadruple(Operator::ASTA, variable, "", tmp));
}

void SemanticAnalyzer::assign(const string& name, const string& value,
    BasicType nametype, BasicType valuetype) {
    string variable = value;
    cut(nametype, valuetype, variable);
    quadruples.addTail(new Quadruple(Operator::ASS, variable, "", name));

}