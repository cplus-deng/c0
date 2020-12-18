#pragma once
#ifndef TYPE_H
#define TYPE_H

#include <string>
#include <vector>
#include <map>
using std::string;
using std::vector;
using std::map;

// �ʷ��������ص��·�������
namespace name_ST {
    enum SymbolType {
        //Key Words
        FN_KW,  
        LET_KW,
        CONST_KW,
        AS_KW,
        WHILE_KW,
        IF_KW,
        ELSE_KW,
        RETURN_KW,
        BREAK_KW,
        CONTINUE_KW,
        INT_KW,
        DOUBLE_KW,
        VOID_KW,

        //function keywords
        GETINT,
        GETDOUBLE,
        GETCHAR,
        PUTINT,
        PUTCHAR,
        PUTDOUBLE,
        PUTSTR,
        PUTLN,

        //LITERAL
        UINT_LITERAL,
        DOUBLE_LITERAL,
        STRING_LITERAL,
        CHAR_LITERAL,

        //basic type
        /*
        INT,
        DOUBLE,
        VOID,
        */

        //IDENTFILER
        IDENT,

        //SYMBOL
        PLUS,
        MINUS,
        MUL,
        DIV,
        ASSIGN,
        EQ,
        NEQ,
        LT,
        GT,
        LE,
        GE,
        L_PAREN,//��Բ����(
        R_PAREN,//��Բ����)
        L_BRACE,//������{
        R_BRACE,//�һ�����}
        SINGLE_QUOTE,       // ������                      
        DOUBLE_QUOTE,       // ˫����
        ARROW,
        COMMA, //����
        COLON, //ð��
        SEMICOLON,//�ֺ�

        //ע��
        COMMENT,
        ILLEGAL,
        FINISH
    };
}

// �˴���ǣ�������ʱ������һЩ����֮�󣬵���һ���﷨�ɷֽ���
// ���磺�ڴ�����ʽʱ����������д������� <���ʽ> �Ľ�β����������� EXPRESSION 
namespace name_EE {
    enum ErrorEnd {
        
        NORMAL,                 // ������������ʾû�д��󣬲���Ҫ����
        PROGRAM,                // ��������
        BLOCK,                  // �ֳ���
        CONST_DECLARE,          // ����˵��
        CONST_DEFINE,           // ��������
        CONST,                  // ����
        CHAR,                   // �ַ�
        STRING,                 // �ַ���
        UNSIGNED,               // �޷�������
        VAR_DECLARE,            // ����˵��
        VAR_DEFINE,             // ��������
        TYPE,                   // ����
        BASIC_TYPE,             // ��������
        PROCEDURE_DECLARE,      // ����˵��
        FUNCTION_DECLARE,       // ����˵��
        PROCEDURE_HEAD,         // �����ײ�
        FUNCTION_HEAD,          // �����ײ�
        FORMAL_PARAMETER_LIST,  // ��ʽ������
        FORMAL_PARAMETER,       // ��ʽ����
        STATEMENT,              // ���
        ASSIGN_STATEMENT,       // ��ֵ���
        FUNCTION_ASSIGN,        // ��������ֵ��ֵ
        EXPRESSION,             // ���ʽ
        TERM,                   // ��
        FACTOR,                 // ����
        CALL_STATEMENT,         // �������
        REAL_PARAMETER_LIST,    // ʵ�ڲ�����
        CONDITION,              // ����
        RELATION_OPERATOR,      // ��ϵ�����
        CONDITION_STATEMENT,    // �������
        WHILE_STATEMENT,     // do-while ���
        BLOCK_STATEMENT,        // block��� ��{}������
        COMPOUND_STATEMENT,     // �������
        READ_STATEMENT,         // read ���
        WRITE_STATEMENT         // write ���
    };
}

// ��ʶ��������
namespace name_IT {
    enum IdentifierType {
        ADDR,  //��ʱ����
        VAR,
        CONST,
        FUNCTION
    };
}

// ��������
namespace name_BT {
    enum BasicType {
        INTEGER,
        DOUBLE,
        VOID
    };
}

// ��������
namespace name_ET {
    enum ErrorType {
        NO_COLON,       //ȱ��ð�� const a:int=1;
        NO_PERIOD,      // ȱ�پ��
        ILLEGAL_CHAR,   // �Ƿ��ַ�
        NO_FINISH,      // ϣ��������û�н���
        FINISH,         // �����в��������ļ�����
        NO_CONST,       // ȱ�� const �ؼ���
        NO_SEMICOLON,   // ȱ�ٷֺ�
        NO_IDENTIFIER,  // ȱ�ٱ�ʶ��
        REDEFINE,       // ��ʶ���ض���
        NO_EQUAL,       // ȱ�ٵȺ�
        NO_CHNUM,       // ȱ���ַ���������<����>��
        NO_SINGLEQUOTE, // ȱ�ٵ�����
        NO_DOUBLEQUOTE, // ȱ��˫����
        ILLEGAL_STRING, // �Ƿ��ַ���
        NO_NUMBER,      // ϣ�������ֶ�����
        INT_OVERFLOW,   // �������
        NO_VAR,         // ȱ�� var �ؼ���
        NO_COLON,       // ȱ��ð��
        NO_LEFTSQUARE,  // ȱ��������
        NO_RIGHTSQUARE, // ȱ���ҷ�����
        NO_OF,          // ȱ�� of �ؼ���
        NO_BASICTYPE,   // ȱ�ٻ�������
        NO_PROCEDURE,   // ȱ�� procedure �ؼ���
        NO_FUNCTION,    // ȱ�� function �ؼ���
        NO_LEFTPAREN, // ȱ��������
        NO_RIGHTPAREN,// ȱ��������
        NO_LEFTBRACE,
        NO_RIGHTBRACE,
        CONSTANT,       // ����ĳ�����ʶ��
        NO_ASSIGN,      // ȱ�ٸ�ֵ������ϣ���Ǹ�ֵ��䣬ʵ�ʲ���
        NO_VALUE,       // ȱ��ֵ
        UNDEFINED,      // ��ʶ��δ����
        CANNOT_CALL,    // ��ʶ���������ں�������̵���
        INT_TO_CHAR,    // ��������ת��Ϊ�ַ���
        TOO_LESS_ARG,   // ����̫��
        TOO_MANY_ARG,   // ����̫��
        UNVARIABLE,     // ��Ҫ�������ʽ
        NO_RELATION,    // ��Ҫ��ϵ�������û��
        NO_IF,          // ȱ�� if �ؼ���
        NO_THEN,        // ȱ�� then �ؼ���
        NO_ELSE,        // ȱ�� else �ؼ���
        NO_DO,          // ȱ�� do �ؼ���
        NO_WHILE,       // ȱ�� while �ؼ���
        NO_FOR,         // ȱ�� for �ؼ���
        NO_DOWN_TO,     // ȱ�� to �� downto �ؼ���
        NO_BEGIN,       // ȱ�� begin �ؼ���
        NO_END,         // ȱ�� end �ؼ���
        NO_READ,        // ȱ�� read �ؼ���
        NO_WRITE        // ȱ�� write �ؼ���
    };
}

// ��Ԫʽ�еĲ�����
namespace name_OP {
    enum Operator {
        ADD,    // �ӷ�
        SUB,    // ����
        MUL,    // �˷�
        DIV,    // ����
        NEG,    // ��
        ASS,    // ��ֵ
        GA,     // ��ȡԪ�صĵ�ַ����������ͨ����������Ԫ��
        ASTA,   // ���ַ�еĶ�����ֵ
        ASFA,   // �ӵ�ַ��ȡ�����ݣ�������������ֵ
        FAS,    // ������������ֵ
        CUT,    // �������ضϳ��ַ���
        LAB,    // ��ǩ
        CMP,    // �Ƚ�
        JG,     // ������ת
        JGE,    // ���ڻ������ת
        JL,     // С����ת
        JLE,    // С�ڻ������ת
        JE,     // ������ת
        JNE,    // ��������ת
        JMP,    // ��������ת
        CALP,   // ���̵��ã���������ֵ
        CALF,   // �������ã�������ֵ
        RTN,    // ��������̷���
        PUSH,   // ѹջ
        RI,     // ������
        RC,     // ���ַ�
        WS,     // д�ַ���
        WI,     // д����
        WC,     // д�ַ�
        NL      // ����
    };
}

using name_ST::SymbolType;
using name_IT::IdentifierType;
using name_BT::BasicType;
using name_EE::ErrorEnd;
using name_ET::ErrorType;
using name_OP::Operator;

// ÿ����������Ϣ
struct Argument {
    string name;    // ������
    BasicType type; // ����
    bool vary;      // true ��ʾ����ַ��false ��ʾ��ֵ

    // ���캯��
    Argument(const string& n, BasicType t, bool v) : name(n), type(t), vary(v) {}
};

// ���ű�ṹ
struct STE {
    string name;                    // ��ʶ������
    IdentifierType type;            // ��ʶ������
    BasicType valueType;            // ������������ֵ�����ͣ�
                                    // ���飺Ԫ�ص����ͣ�
                                    // ����������ֵ����
    int multiInt;                   // ������ֵ��
                                    // ���飺������
                                    // ���� / ���̣���ʱ�����ĸ���
    int level;                      // ��ǰ���ڵڼ���
    int location;                   // ����/����������ջ�ϵ���Ե�ַ
    vector<Argument>* arguments;    // ����/���̵Ĳ����б�                
    ////////////////////////////////////////////////////////////////
private:    // ���£�Ϊ�������η��ű�������ָ��
    STE* father;                    // ��ǰ���ŵĸ��ڵ㣨��������/���̣�
    map<string, STE*>* childs;     // ��ǰ����(����/����)�µķ���
    friend class SymbolTableManager;// �����ݽṹ��ص�ָ�뱩¶�����ű������
    ////////////////////////////////////////////////////////////////
public:
    STE() : arguments(NULL),
        childs(NULL),
        location(0),
        father(NULL) {}     // ���캯�����������б���Ϊ NULL
    ~STE() {
        delete childs;      // ��ɾ����������ɾ����ǰ�ڵ�
        delete arguments;
    }                       // ������������ɾ�������б�
                            // ���ã�����ʱ��ɾ���Ե�ǰ�ڵ�Ϊ������������
    STE(const STE& ste) {
        name = ste.name;
        type = ste.type;
        valueType = ste.valueType;
        multiInt = ste.multiInt;
        level = ste.level;
        location = ste.location;
        // �����ָ��Ҫ����С�ģ�����
        arguments = ste.arguments == NULL ? NULL :
            new vector<Argument>(*ste.arguments);
        // ���ݽṹ��ص�ָ�������Ժ���
        father = NULL;
        childs = NULL;
    }                       // ��������
};

static string stringPlusInt(const string& str, int n) {
    string newstr;
    char num[20];
    newstr = str;
    sprintf(num, "%d", n);
    newstr.append(num);
    return newstr;
}

#endif // !TYPE_H
