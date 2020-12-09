#pragma once
#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H
#include <fstream>
#include <string>
#include "type.h"
using std::string;
using std::ifstream;

// �ʷ�������
class LexicalAnalyzer {
public:
    static LexicalAnalyzer &initialLexicalAnalyzer(ifstream &file); 
                                    // ����ģʽ��ԭ���ǣ�
                                    //  token.symbol ������̬���� strGot
                                    //  ��� LexicalAnalyzer.cpp
    bool overflow;          // ������
    void nextSymbol();      // ��ȡ��һ�����ţ����������ʹ��� symbolType��
                            //  ����ķ��Ŵ��� symbol
    void nextChar();        // ��ȡ��һ�� char ���͵��ַ���
                            //  ����ǺϷ��� char���� symbolType 
                            //  ��Ϊ CHARATER�������� ASCII ֵ���� token.number
                            //  ������ǺϷ��� char���� symbolType ��Ϊ ILLEGAL
    void nextString();      // ��ȡ��һ���ַ���������˫���ţ������Ƿ��ַ���˫���Ž�ֹ��
                            //  ����ֹ֮ǰ�Ĳ��ִ��� token.symbol������Ϊ�մ�
                            //  ��symbolType ��Ϊ STRING

private:
    ifstream &sourceFile;           // Դ�����ļ�������
    int lineNumber;                 // �·�����Դ�����е��к�
    SymbolType symbolType;          // �·��ŵ�����
    union Token {                   // �·��ŵ����ݣ�ʹ��������������Ϊ��ͬ���ŵ����Ͳ�ͬ
        int number;         // �����͵�����ֵ
        string *symbol;     // �ַ������͵��ַ���ָ��
    } token;                 
    friend class GrammarAnalyzer;   // ��Ԫ�ࣺ�﷨������
                                    //  �﷨������Ҫֱ�ӻ�ȡ�·��ŵ����ͺ�����
    friend class ErrorHandler;      // ��Ԫ�ࣺ��������
                                    //  ������ʱ skip ������Ҫ��ȡ�·��ŵ�����
    friend class SemanticAnalyzer;  // ��Ԫ�ࣺ���������
                                    //  �����������Ҫ֪���·��ŵ����ͺ�����
                                        
    LexicalAnalyzer(ifstream &file);            // ˽�л����캯��������ģʽ
                                                //  ����ΪԴ�ļ��� ifstream ����
                                                //  ��������������Ч��
    LexicalAnalyzer(const LexicalAnalyzer &);   // ˽�л��������죬����ģʽ����ֹ����
    LexicalAnalyzer &operator= (const LexicalAnalyzer &);
                                                // ˽�л����ص� = ���������ֹ��ֵ
};

#endif // !LEXICAL_ANALYZER_H
