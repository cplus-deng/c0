#pragma once
/*
    ErrorHandler.h
    ���� ������������ �༰����صĺ�
*/

#ifndef ERROR_HANDLER__H
#define ERROR_HANDLER__H

#include "LexicalAnalyzer.h"
#include <list>
using std::list;
using std::pair;

class ErrorHandler {
public:
    // �������
    int errorCount;
    int warningCount;
    void addEndmap(SymbolType, ErrorEnd);   // �� endmap ���һ��
    void removeEndmap(SymbolType);          // �� endmap ���Ƴ� SymbolType�����û�У��޲���
    // ����ģʽ
    static ErrorHandler& initialErrorHandler(LexicalAnalyzer&, bool warning);
    // ��������
    void printError(ErrorType);
    void printError(ErrorType, const string&);
    ErrorEnd skip();
    // ���溯��
    void printWarning(ErrorType err);
    void printWarning(ErrorType err, const string& msg);

private:
    // skip �����ɽ���ʱ�� SymbolType ������ֵ���ձ�
    list<pair<SymbolType, ErrorEnd> > endmap;
    // �ʷ�������
    LexicalAnalyzer& lexicalAnalyzer;
    // ���濪��
    bool warning;
    // ����ģʽ
    ErrorHandler(LexicalAnalyzer&, bool warning);
    ErrorHandler(const ErrorHandler&);
    ErrorHandler& operator= (const ErrorHandler&);
};

#endif // !ERROR_HANDLER__H
