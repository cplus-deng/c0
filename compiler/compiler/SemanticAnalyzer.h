#pragma once
/*
    SemanticAnalyzer.h
    ��������� ������
*/
#ifndef SEMANTIC_ANALYZER__H
#define SEMANTIC_ANALYZER__H

#include "SymbolTableManager.h"
#include "ErrorHandler.h"
#include "Quadruples.h"
#include "LexicalAnalyzer.h"

class SemanticAnalyzer {
public:
    // ����ģʽ
    static SemanticAnalyzer& initialSemanticAnalyzer(
        ErrorHandler&, SymbolTableManager&, Quadruples&, LexicalAnalyzer&);

    // ���£��ṩ�����嶯�����������﷨����������

    // ���£��ֱ���ÿ���﷨�ɷַ�����ͷ�ͽ�βʹ�ã��� skip ������ʹ��
    void programEndmapInit();
    void blockEndmapInit();
    void blockEndmapClear();
    void constDeclareEndmapInit();
    void constDeclareEndmapClear();
    void constDefineEndmapInit();
    void constDefineEndmapClear();
    void charEndmapInit();
    void charEndmapClear();
    void stringEndmapInit();
    void stringEndmapClear();
    void varDeclareEndmapInit();
    void varDeclareEndmapClear();
    void varDefineEndmapInit();
    void varDefineEndmapClear();
    void procedureDeclareEndmapInit();
    void procedureDeclareEndmapClear();
    void functionDeclareEndmapInit();
    void functionDeclareEndmapClear();
    void functionDeclareSkip();
    void procedureHeadEndmapInit();
    void procedureHeadEndmapClear();
    void functionHeadEndmapInit();
    void functionHeadEndmapClear();
    void formalParameterEndmapInit();
    void formalParameterEndmapClear();
    void statementEndmapInit();
    void statementEndmapClear();
    void termEndmapInit();
    void termEndmapClear();
    void factorEndmapInit();
    void factorEndmapClear();

    // ���£��� skip ֮��õ��ķ��ţ��еĿ���Ҫ����������ţ������ﴦ��
    void constDeclareSkip();
    void charSkip();
    void stringSkip();

    // ����������ز���
    void constDefine(const string&, BasicType, int value);
    string& stringDefine(const string&);
    void varDefine(const string&, BasicType);
    void functionDefine(const string& funcname, vector<Argument>& args, BasicType type);
    void formalParameterAdd(vector<string>& names, BasicType type,
        bool vary, vector<Argument>&);
    void getAddress(const string& base, const string& offset, string& addr);
    void getContent(const string& base, string& value);
    void assignFromAddress(const string& addr, const string& value,
        BasicType addrType, BasicType valueType);
    void assignToAddress(const string& addr, const string& value,
        BasicType addrType, BasicType valueType);
    void assignToArray(const string& arrname, const string& offset, const string& value,
        BasicType arraytype, BasicType valuetype);
    void assign(const string& name, const string& value,
        BasicType nametype, BasicType valuetype);
    void negative(const string& oper1, string& dest);
    void addition(const string& oper1, const string& oper2, string& dest);
    void subtraction(const string& oper1, const string& oper2, string& dest);
    void multiplicatioin(const string& oper1, const string& oper2, string& dest);
    void division(const string& oper1, const string& oper2, string& dest);
    void intToString(int n, string& str);
    void callFunction(const string& name, string& value);
    void callProcedure(const string& name);
    void functionAssign(const string& name, const string& value,
        BasicType functype, BasicType valuetype);
    void label(const string& lab);
    void funcprocStart();
    void funcprocReturn();
    void push(const string& value);
    void passParameter(const string& value, BasicType formaltype, BasicType realtype);
    void passAddress(const string& addr, BasicType formaltype, BasicType realtype);
    void compare(const string& value1, const string& value2);
    void dissatisfyJump(SymbolType op, const string& lab);
    void satisfyJump(SymbolType op, const string& lab);
    void jumpAnyway(string lab);
    string& newLabel();
    string& newLabel(const string& forehead);
    void readChar(const string& name);
    void readInteger(const string& name);
    void writeString(const string& name);
    void writeInteger(const string& value);
    void writeChar(const string& ch);
    void newline();

    // Ҫ��������Ԫʽ�󣬼�¼ÿ��������������˶��ٸ���ʱ�����������ṩ���������
    // ��ѯ temporaryCount ��ֵ���� ����/���� ��ͷ get һ�Σ���β get һ�Σ�
    // ����֮�������� ����/���� �õ�����ʱ�����ĸ�����
    int getTemporaryCount() { return temporaryCount; }

private:
    ErrorHandler& errorHandler;
    SymbolTableManager& table;
    Quadruples& quadruples;
    LexicalAnalyzer& lexicalAnalyzer;
    // ��ʱ��������
    int temporaryCount;
    string newTemporary();
    // label ����
    int labelCount;
    // ������ͣ���Ҫʱ�ض�
    void cut(BasicType desttype, BasicType valuetype, string& value);
    // ����ģʽ
    SemanticAnalyzer(ErrorHandler&, SymbolTableManager&, Quadruples&, LexicalAnalyzer&);
    SemanticAnalyzer(const SemanticAnalyzer&);
    SemanticAnalyzer& operator= (const SemanticAnalyzer&);
};

#endif // !SEMANTIC_ANALYZER__H
