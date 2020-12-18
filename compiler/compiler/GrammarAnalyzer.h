#pragma once
/*
    GrammarAnalyzer.h
    �ṩ�﷨��������� calss ����
*/
#ifndef GRAMMA_ANALYZER__H
#define GRAMMA_ANALYZER__H

#include <fstream>
#include "LexicalAnalyzer.h"
#include "SemanticAnalyzer.h"
using std::ofstream;

// �﷨������
class GrammarAnalyzer {
private:
    // ����ģʽ
    GrammarAnalyzer(LexicalAnalyzer&, ErrorHandler&,
        SymbolTableManager&, SemanticAnalyzer&);
    GrammarAnalyzer(const GrammarAnalyzer&);
    GrammarAnalyzer& operator= (const GrammarAnalyzer&);
public:
    static GrammarAnalyzer& initialGrammarAnalyzer(LexicalAnalyzer&, ErrorHandler&,
        SymbolTableManager&, SemanticAnalyzer&);

    void grammarAnalyze();

private:
    LexicalAnalyzer& lexicalAnalyzer;   // �ʷ����������﷨����ʹ��
    SemanticAnalyzer& semanticAnalyzer; // ���������
    ErrorHandler& errorHandler;         // ��������
    SymbolTableManager& table;          // ���ű������

    // ���£����﷨�ɷֵķ���
    void blockAnalyze();

    void constDeclareAnalyze();
    void constDefineAnalyze();
    void constAnalyze(int&, BasicType&);
    void constAnalyze(double&, BasicType&);
    void varDeclareAnalyze();
    void varDefineAnalyze();
    void varAnalyze(int&, BasicType&);

    void charAnalyze(int&);
    void stringAnalyze(string&);
    void unsignedAnalyze(int&);
    void floatAnalyze(double&);
    void typeAnalyze(IdentifierType&, BasicType&, int&);
    void basicTypeAnalyze(BasicType&);
    void procedureDeclareAnalyze();
    void functionDeclareAnalyze();
    void procedureHeadAnalyze(string&);
    void functionHeadAnalyze(string&);
    void formalParameterListAnalyze(vector<Argument>&);
    void formalParameterAnalyze(vector<Argument>&);
    void statementAnalyze();
    void assignStatementAnalyze();
    void functionAssignStatementAnalyze(const string& name);
    void expressionAnalyze(string& value, BasicType& type, bool& variable, bool addr);
    void termAnalyze(string& value, BasicType& type, bool& variable, bool addr);
    void factorAnalyze(string& value, BasicType& type, bool& variable, bool addr);
    void callStatementAnalyze(string& value, BasicType& type);
    void realParameterListAnalyze(const string& name);
    void conditionAnalyze(SymbolType& relationOp);
    void relationOperatorAnalyze(SymbolType& relationOp);
    void conditionStatementAnalyze();
    void doWhileStatementAnalyze();
    void forStatementAnalyze();
    void blockStatementAnalyze();
    void compoundStatementAnalyze();
    void readStatementAnalyze();
    void writeStatementAnalyze();

};

#endif // !GRAMMA_ANALYZER__H