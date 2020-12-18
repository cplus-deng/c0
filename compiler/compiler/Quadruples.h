#pragma once
/*
    Quadruples.h
    �ṹ������Ԫʽ
    ������ʽ��֯
*/
#ifndef QUADRUPLES__H
#define QUADRUPLES__H

#include "type.h"
#include <fstream>
using std::ofstream;

struct Quadruple {
    Operator op;
    string oper1;
    string oper2;
    string dest;
    Quadruple(Operator ope, const string& o1, const string& o2, const string& d) :
        op(ope), oper1(o1), oper2(o2), dest(d), prenode(NULL), sucnode(NULL) {}
private:
    // ������ṹ��ص�ָ����
    Quadruple* prenode; // ǰ���ڵ�
    Quadruple* sucnode; // ��̽ڵ�
    // ��Ԫ�࣬���Բ�������ṹ
    friend class Quadruples;
};

class Quadruples {
public:
    void addTail(Quadruple*);      // ����β��һ����Ԫʽ
    void output(ofstream&);        // �����Ԫʽ���ļ�
    Quadruples();                   // ���캯��
    ~Quadruples();                  // ��������
    class iterator {
    private:
        Quadruple* point;
        iterator(Quadruple* p) : point(p) {}
        friend class Quadruples;
    public:
        bool operator== (const iterator& b) const {
            return point == b.point;
        }
        bool operator!= (const iterator& b) const {
            return point != b.point;
        }
        iterator& operator++ (int i) {
            return operator++();
        }
        iterator& operator++ () {
            point = point->sucnode;
            return *this;
        }
        iterator& operator-- (int i) {
            return operator--();
        }
        iterator& operator-- () {
            point = point->prenode;
            return *this;
        }
        Quadruple& operator* () {
            return *point;
        }
        Quadruple* operator-> () {
            return point;
        }
    };
    iterator& begin() {
        static iterator it(head);
        it.point = head->sucnode;
        return it;
    }
    iterator& end() {
        static iterator it(NULL);
        return it;
    }

private:
    Quadruple* head;    // ��Ԫʽ�����ͷ�ڵ�
    Quadruple* tail;    // ��Ԫʽ�����β�ڵ�
    friend class TargetCodeGenerator;
    // ���ÿ�������
    Quadruples(const Quadruples&);
};

#endif // !QUADRUPLES__H
