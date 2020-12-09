/*
    LexicalAnalyzer.cpp
    词法分析器的实现。
*/
#include "LexicalAnalyzer.h"
#include <cctype>
using namespace std;
using namespace name_ST;

static string strGot;   // 读到的字符串
static char ch;         // 下一个即将读出来的（还没读到它）的字符
                        //  设这个变量的目的是提高效率

static map<string, SymbolType> reservedWords;   // 保留字表，建立每个保留字与其类型之前的关系

// 单例模式，对外提供的“构造函数”
LexicalAnalyzer& LexicalAnalyzer::initialLexicalAnalyzer(ifstream& file) {
    static LexicalAnalyzer theLexicalAnalyzer(file);
    return theLexicalAnalyzer;
}

// 真正的构造函数
LexicalAnalyzer::LexicalAnalyzer(ifstream& file) : sourceFile(file) {
    // 将文件输入流置为参数
// 初始化行号为 1
    lineNumber = 1;
    // 读进来文件中的第一个字符备用
    ch = sourceFile.get();
    //初始化保留字表                          
#define INSERT_RES(str, type) reservedWords.insert(pair<string, SymbolType>(str, type))   // 定义宏，简化填表代码
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
#undef  INSERT_RES                                                  // 取消定义 INSERT_RES 宏
}

#define GET     ch = sourceFile.get()   // 定义宏，简化从文件中读取一个字符的操作
#define PEEK    ch = sourceFile.peek()  // 定义宏，简化查看文件流第一个字符的操作
#define ADD(c)  strGot.append(1, c)     // 定义宏，简化往字符串结尾添加字符的操作

// 读取下一个符号，将符号类型存入 symbolType，具体的符号存入 symbol
void LexicalAnalyzer::nextSymbol() {
    while (true) {
        switch (ch) {
        case '\n':
            lineNumber++;           // 记行号
            //------------------------没有 break;
        case ' ': case '\t':
            GET;                    // 跳过空白符
            continue;
        default:
            goto END_SPACE;
        }
    }
END_SPACE:
    /*
    ** 未具体考虑溢出问题
    */
    if (isdigit(ch)) {          // 遇数字，这里读到的一定是整数     
        symbolType = UINT_LITERAL;                // 置符号类型为“数字”        
        token.number = ch - '0';            // 将数字字符转化为整数
        overflow = false;
        for (GET; isdigit(ch); GET) {
            int tmp = token.number;
            token.number = token.number * 10 + ch - '0';
            // 将数字字符转化为整数，更新 number 的值
            if ((token.number - (ch - '0')) / 10 != tmp)    // 溢出
                overflow = true;
        }
        // 这里，循环跳出时，ch 是非数字字符
        //  由于 ch 是还未处理的字符，所以逻辑正确
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
    else if (isalpha(ch)) {   // 遇字母，这里读到的一定是标识符或保留字
        token.symbol = &strGot;             // 这时读到的符号是一定要存入 token.symbol 的
        strGot.clear();                     // 先清空字符串
        ADD(ch);                            // 先将第一个字符存入字符串，以备后面再读
        for (GET; isalnum(ch); GET)
            ADD(ch);
        // 这里，循环跳出时，ch 是一个新的未处理的字符，逻辑正确

        // 以下，判断读到的符号是标识符还是保留字，将其具体类型存入 symbolType
        map<string, SymbolType>::iterator it = reservedWords.find(strGot);
        if (it == reservedWords.end())      // 不在保留字表中，说明是标识符
            symbolType = IDENT;
        else                                // 在保留字表中
            symbolType = it->second;        // 要把保留字的具体类型存入 symbol
    }
    else {                    // 遇其他符号，根据符号本身判断符号的类型
                             //  每种符号唯一对应一种类型，所以不必存入 token
        switch (ch) {
            // 以下，在这个 switch 块中，每一种情况执行完之后，都使得 ch 是处理完的字符
            // 在这个 switch 块外面，统一再读一个新字符备用
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
        // 以上，switch 块出来之后，ch 为处理过的字符
        GET;  // 此处需要再 get 一个新的字符备用
    }
}

// 读取一个字符串，不含双引号，遇到非法字符或双引号截止，
//  将截止之前的部分存入 token.symbol，可能为空串
//  将symbolType 置为 STRING
void LexicalAnalyzer::nextString() {
    token.symbol = &strGot; // 令 token.symbol 指向新读的字符串
    strGot.clear();         // 清空字符串以备用       
    while (ch == 32 || ch == 33 || (ch >= 35 && ch <= 126)) {
        ADD(ch);
        GET;
    }
    // 这里，循环跳出时，ch 是一个新的未处理的字符，逻辑正确
    if (ch != '\"') {           // 读到非法字符
        symbolType = ILLEGAL;   // 返回 ILLEGAL 类型
        strGot.clear();
        ADD(ch);                // 将非法的字符存入 token 中，
                                //  此时 token 中只有这一个非法字符
        GET;                    // 不要忘了再读一个备用！！！
    }
    else
        symbolType = STRING;    // 正常，返回 STRING
}

// 获取下一个 char 类型的字符，
// 如果是合法的 char，将 symbolType 置为 CHARATER，
//  并将其 ASCII 值存入 token.number
// 如果不是合法的 char，将 symbolType 置为 ILLEGAL
void LexicalAnalyzer::nextChar() {
    while (true) {
        switch (ch) {
        case '\n':
            lineNumber++;           // 记行号
            //------------------------没有 break;
        case ' ': case '\t':
            GET;                    // 跳过空白符
            continue;
        default:
            goto END_SPACE;
        }
    }
END_SPACE:
    if (isalnum(ch)) {          // 是合法字符，即字母或数字字符
        symbolType = CHARATER;
        token.number = ch;      // 将字符的 ASCII 值存入 token.number
    }
    else {                    // 非法字符
        symbolType = ILLEGAL;
        strGot.clear();
        ADD(ch);
        token.symbol = &strGot; // 将非法字符以字符串形式返回
    }
    GET;                        // get 一个新的字符备用！！
}