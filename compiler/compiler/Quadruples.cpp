/*
    Quadruples.cpp
    ��Ԫʽʵ��
*/
#include "Quadruples.h"
using namespace std;

Quadruples::Quadruples() {
    head = new Quadruple(Operator::LAB, "Hello", "", "");
    tail = head;
}

Quadruples::~Quadruples() {
    while (head != NULL) {
        tail = head->sucnode;
        delete head;
        head = tail;
    }
}

ostream& operator<< (ostream& os, Operator op) {
#define case(op) case Operator::op: \
    os << #op; break
    switch (op) {
        case(ADD);    // �ӷ�
            case(SUB);    // ����
                case(MUL);    // �˷�
                    case(DIV);    // ����
                        case(NEG);    // ��
                            case(ASS);    // ��ֵ
                                case(GA);     // ��ȡ��ַ
                                    case(ASFA);   // �ӵ�ַ��ȡ����������Ķ�����ֵ
                                        case(ASTA);   // ����ַ�еĶ�����ֵ
                                            case(FAS);    // ������������ֵ
                                                case(CUT);    // �������ضϳ��ַ���
                                                    case(LAB);    // ��ǩ
                                                        case(CMP);    // �Ƚ�
                                                            case(JG);     // ������ת
                                                                case(JGE);    // ���ڻ������ת
                                                                    case(JL);     // С����ת
                                                                        case(JLE);    // С�ڻ������ת
                                                                            case(JE);     // ������ת
                                                                                case(JNE);    // ��������ת
                                                                                    case(JMP);    // ��������ת
                                                                                        case(CALP);   // ���̵��ã���������ֵ
                                                                                            case(CALF);   // �������ã�������ֵ
                                                                                                case(RTN);    // ��������̷���
                                                                                                    case(PUSH);   // ѹջ
                                                                                                        case(RI);     // ������
                                                                                                            case(RC);     // ���ַ�
                                                                                                                case(WS);     // д�ַ���
                                                                                                                    case(WI);     // д����
                                                                                                                        case(WC);     // д�ַ�
                                                                                                                            case(NL);     // ����
                                                                                                                            default:
                                                                                                                                break;
    }
    return os;
#undef case
}

void Quadruples::addTail(Quadruple* quadruple) {
    quadruple->prenode = tail;
    quadruple->sucnode = tail->sucnode;
    tail->sucnode = quadruple;
    tail = quadruple;
    /////////////////////////////////////
    //    static ofstream ofs("fuck~~~~~~~~~~~~~~~~~~~~.txt");
    //    Quadruple *pq = quadruple;
    //    ofs << pq->op << " ";
    //    bool out = false;
    //    if (!pq->oper1.empty()) {
    //        ofs << pq->oper1;
    //        out = true;
    //    }
    //    if (!pq->oper2.empty()) {
    //        ofs << (out ? ", " : "");
    //        ofs << pq->oper2;
    //        out = true;
    //    }
    //    if (!pq->dest.empty()) {
    //        ofs << (out ? ", " : "");
    //        ofs << pq->dest;
    //    }
    //    ofs << endl;
    /////////////////////////////////////
}

void Quadruples::output(ofstream& ofs) {
    Quadruple* pq;
    for (pq = head->sucnode; pq != NULL; pq = pq->sucnode) {
        ofs << pq->op << " ";
        bool out = false;
        if (!pq->oper1.empty()) {
            ofs << pq->oper1;
            out = true;
        }
        if (!pq->oper2.empty()) {
            ofs << (out ? ", " : "");
            ofs << pq->oper2;
            out = true;
        }
        if (!pq->dest.empty()) {
            ofs << (out ? ", " : "");
            ofs << pq->dest;
        }
        ofs << endl;
    }
}