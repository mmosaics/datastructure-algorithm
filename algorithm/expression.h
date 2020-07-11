//
// Created by 胡凌瑞 on 2020/7/11.
//

#ifndef DATA_STRUCTURE_CPP_EXPRESSION_H
#define DATA_STRUCTURE_CPP_EXPRESSION_H

#include <string>
#include <stack>
#include <utility>
#include <vector>
#include <sstream>

using std::string;
using std::stack;
using std::vector;

/**
 * 把一个中缀表达式转换为逆波兰表达式
 * @param expression
 */

/**
 * 用于返回运算符的优先级
 * @param operatorChar
 * @param hasBracket
 * @return
 */
int priorityOfOperation(char operatorChar, bool & hasBracket)
{
    //栈中不存在左括号
    if(!hasBracket) {
        switch (operatorChar) {
            case '(':
                return 7;
            case ')':
                return 1;
            case '-':
            case '+':
                return 3;
            case '*':
            case '/':
                return 5;
            default:
                return -1;
        }
    } else {
        switch (operatorChar) {
            case '(':
                return 1;
            case ')':
                return 7;
            case '-':
            case '+':
                return 3;
            case '*':
            case '/':
                return 5;
            default:
                return -1;

        }
    }
}

/**
 * 一个简单的标示结构体，用于指示分出来的词是数字还是运算符，相当简陋
 */
enum Type {NUMBER, OPERATOR};
struct Lexeme {
    string data;
    Type type;

    Lexeme(string d, Type t):data(std::move(d)), type(t) {}

    Lexeme(string && d, Type t):data(std::move(d)), type(t){}

};

/**
 * 判断一个字符是否为操作符，目前暂定操作符为 + - * / ( )
 * @param operatorChar
 * @return
 */
bool isOperator(const char & operatorChar) {
    return operatorChar == '+' || operatorChar == '-' ||
           operatorChar == '*' || operatorChar == '/' ||
           operatorChar == '(' || operatorChar == ')';
}

/**
 * 分割一个字符串，把分割后的结果存入vector中，暂时只实现按照某个字符分割
 * @param expression
 * @param splitter
 * @param stringAfterSplit
 */
void splitString(const string & expression, const char & splitter, vector<Lexeme *> & stringAfterSplit) {

    string newString;
    Lexeme * p;

    for(char i : expression) {
        if(i != splitter) {
            newString.insert(newString.end(), i);
            if(isOperator(i))
                p = new Lexeme(newString, OPERATOR);
            else
                p = new Lexeme(newString, NUMBER);

        }
        else {
            stringAfterSplit.push_back(p);
            newString.erase(newString.begin(), newString.end());
        }
    }

    //把最后的字符也加入数组
    stringAfterSplit.push_back(p);

}

string reversePolishNotation(const string & expression)
{
    stack<char> operationStack;
    string suffixExpression;
    bool hasBracket = false;

    for(int i = 0; i < expression.length(); ++i) {


    }

}


#endif //DATA_STRUCTURE_CPP_EXPRESSION_H
