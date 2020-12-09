/*
    LexicalAnalyzer.cpp
    �ʷ���������ʵ�֡�
*/
#include "LexicalAnalyzer.h"
#include <cctype>
using namespace std;
using namespace name_ST;

static string strGot;   // �������ַ���
static char ch;         // ��һ�������������ģ���û�����������ַ�
                        //  �����������Ŀ�������Ч��

static map<string, SymbolType> reservedWords;   // �����ֱ�����ÿ����������������֮ǰ�Ĺ�ϵ

// ����ģʽ�������ṩ�ġ����캯����
LexicalAnalyzer& LexicalAnalyzer::initialLexicalAnalyzer(ifstream& file) {
    static LexicalAnalyzer theLexicalAnalyzer(file);
    return theLexicalAnalyzer;
}

// �����Ĺ��캯��
LexicalAnalyzer::LexicalAnalyzer(ifstream& file) : sourceFile(file) {
    // ���ļ���������Ϊ����
// ��ʼ���к�Ϊ 1
    lineNumber = 1;
    // �������ļ��еĵ�һ���ַ�����
    ch = sourceFile.get();
    //��ʼ�������ֱ�                          
#define INSERT_RES(str, type) reservedWords.insert(pair<string, SymbolType>(str, type))   // ����꣬��������
    INSERT_RES("fn", FN_KW);
    INSERT_RES("let", LET_KW);
    INSERT_RES("const", CONST_KW);
    INSERT_RES("as", AS_KW);
    INSERT_RES("while", WHILE_KW);
    INSERT_RES("if",IF_KW);
    INSERT_RES("else", ELSE_KW);
    INSERT_RES("return", RETURN_KW);
    INSERT_RES("break", BREAK_KW);
    INSERT_RES("continue", CONTINUE_KW);
#undef  INSERT_RES                                                  // ȡ������ INSERT_RES ��
}

#define GET     ch = sourceFile.get()   // ����꣬�򻯴��ļ��ж�ȡһ���ַ��Ĳ���
#define PEEK    ch = sourceFile.peek()  // ����꣬�򻯲鿴�ļ�����һ���ַ��Ĳ���
#define ADD(c)  strGot.append(1, c)     // ����꣬�����ַ�����β����ַ��Ĳ���

// ��ȡ��һ�����ţ����������ʹ��� symbolType������ķ��Ŵ��� symbol
void LexicalAnalyzer::nextSymbol() {
    while (true) {
        switch (ch) {
        case '\n':
            lineNumber++;           // ���к�
            //------------------------û�� break;
        case ' ': case '\t':
            GET;                    // �����հ׷�
            continue;
        default:
            goto END_SPACE;
        }
    }
END_SPACE:
    /*
    ** δ���忼���������
    */
    if (isdigit(ch)) {          // �����֣����������һ��������     
        symbolType = UINT_LITERAL;                // �÷�������Ϊ�����֡�        
        token.number = ch - '0';            // �������ַ�ת��Ϊ����
        overflow = false;
        for (GET; isdigit(ch); GET) {
            int tmp = token.number;
            token.number = token.number * 10 + ch - '0';
            // �������ַ�ת��Ϊ���������� number ��ֵ
            if ((token.number - (ch - '0')) / 10 != tmp)    // ���
                overflow = true;
        }
        // ���ѭ������ʱ��ch �Ƿ������ַ�
        //  ���� ch �ǻ�δ������ַ��������߼���ȷ
        if (ch == '.') {
            symbolType = DOUBLE_LITERAL;
            token.float_number = (ch - '0') / 10;
            int place = 2;
            for (GET; isdigit(ch); GET,place++) {
                token.float_number = token.float_number + (ch - '0') / (10 * place);
            }
            token.float_number += token.number;
        }
    }
    else if (isalpha(ch)) {   // ����ĸ�����������һ���Ǳ�ʶ��������
        token.symbol = &strGot;             // ��ʱ�����ķ�����һ��Ҫ���� token.symbol ��
        strGot.clear();                     // ������ַ���
        ADD(ch);                            // �Ƚ���һ���ַ������ַ������Ա������ٶ�
        for (GET; isalnum(ch); GET)
            ADD(ch);
        // ���ѭ������ʱ��ch ��һ���µ�δ������ַ����߼���ȷ

        // ���£��ж϶����ķ����Ǳ�ʶ�����Ǳ����֣�����������ʹ��� symbolType
        map<string, SymbolType>::iterator it = reservedWords.find(strGot);
        if (it == reservedWords.end())      // ���ڱ����ֱ��У�˵���Ǳ�ʶ��
            symbolType = IDENT;
        else                                // �ڱ����ֱ���
            symbolType = it->second;        // Ҫ�ѱ����ֵľ������ʹ��� symbol
    }
    else {                    // ���������ţ����ݷ��ű����жϷ��ŵ�����
                             //  ÿ�ַ���Ψһ��Ӧһ�����ͣ����Բ��ش��� token
        switch (ch) {
            // ���£������ switch ���У�ÿһ�����ִ����֮�󣬶�ʹ�� ch �Ǵ�������ַ�
            // ����� switch �����棬ͳһ�ٶ�һ�����ַ�����
        case '(':
            symbolType = L_PAREN;
            break;
        case ')':
            symbolType = R_PAREN;
            break;
        case '{':
            symbolType = L_BRACE;
            break;
        case '}':
            symbolType = R_BRACE;
            break;
        case ',':
            symbolType = COMMA;
            break;
        case ';':
            symbolType = SEMICOLON;
            break;
        case '+':
            symbolType = PLUS;
            break;
        case '-':
            PEEK;
            if (ch == '>') {
                GET;
                symbolType = ARROW;
            }
            else 
                symbolType = MINUS;
            break;
        case '*':
            symbolType = MUL;
            break;
        case '/':
            symbolType = DIV;
            break;
        case '=':
            PEEK;                         
            if (ch == '=') {
                GET;                      
                symbolType = EQ;
            }
            else
                symbolType = ASSIGN;    
            break;
        case '!':
            PEEK;
            if (ch == '=') {
                GET;
                symbolType = NEQ;
            }
            else {
                symbolType = ILLEGAL;
                token.symbol = &strGot;
                strGot.clear();
                ADD(ch);
            }
            break;
        case '<':
            PEEK;
            if (ch == '=') {
                GET;
                symbolType = LE;
            }
            else
                symbolType = LT;
            break;
        case '>':
            PEEK;                         
            if (ch == '=') {
                GET;                      
                symbolType = GE;
            }
            else
                symbolType = GT;       
            break;
        case EOF:                       
            symbolType = FINISH;
            break;
        default:                            
            symbolType = ILLEGAL;
            token.symbol = &strGot;
            strGot.clear();
            ADD(ch);
            break;
        }
        // ���ϣ�switch �����֮��ch Ϊ��������ַ�
        GET;  // �˴���Ҫ�� get һ���µ��ַ�����
    }
}

// ��ȡһ���ַ���������˫���ţ������Ƿ��ַ���˫���Ž�ֹ��
//  ����ֹ֮ǰ�Ĳ��ִ��� token.symbol������Ϊ�մ�
//  ��symbolType ��Ϊ STRING
void LexicalAnalyzer::nextString() {
    token.symbol = &strGot; // �� token.symbol ָ���¶����ַ���
    strGot.clear();         // ����ַ����Ա���       
    while (ch == 32 || ch == 33 || (ch >= 35 && ch <= 126)) {
        ADD(ch);
        GET;
    }
    // ���ѭ������ʱ��ch ��һ���µ�δ������ַ����߼���ȷ
    if (ch != '\"') {           // �����Ƿ��ַ�
        symbolType = ILLEGAL;   // ���� ILLEGAL ����
        strGot.clear();
        ADD(ch);                // ���Ƿ����ַ����� token �У�
                                //  ��ʱ token ��ֻ����һ���Ƿ��ַ�
        GET;                    // ��Ҫ�����ٶ�һ�����ã�����
    }
    else
        symbolType = STRING;    // ���������� STRING
}

// ��ȡ��һ�� char ���͵��ַ���
// ����ǺϷ��� char���� symbolType ��Ϊ CHARATER��
//  ������ ASCII ֵ���� token.number
// ������ǺϷ��� char���� symbolType ��Ϊ ILLEGAL
void LexicalAnalyzer::nextChar() {
    while (true) {
        switch (ch) {
        case '\n':
            lineNumber++;           // ���к�
            //------------------------û�� break;
        case ' ': case '\t':
            GET;                    // �����հ׷�
            continue;
        default:
            goto END_SPACE;
        }
    }
END_SPACE:
    if (isalnum(ch)) {          // �ǺϷ��ַ�������ĸ�������ַ�
        symbolType = CHARATER;
        token.number = ch;      // ���ַ��� ASCII ֵ���� token.number
    }
    else {                    // �Ƿ��ַ�
        symbolType = ILLEGAL;
        strGot.clear();
        ADD(ch);
        token.symbol = &strGot; // ���Ƿ��ַ����ַ�����ʽ����
    }
    GET;                        // get һ���µ��ַ����ã���
}