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
 * 把一个中缀表达式转换为逆波兰表达式，感觉还有可以完善的地方
 * 目前必须把表达式以空格分割，主要考虑到有数字的情况，以后可以考虑优化一下
 * @param expression
 */

/**
 * 用于返回运算符的优先级
 *  优先级分为栈内优先级和栈外优先级
 *  同一类型的符号，栈外优先级要低于栈内优先级
 *  是因为在向栈内加符号时，比如+号需要添加进去，此时栈内也是+号，则应该先把栈内的+号弹出，再把这个+号入栈
 * @param operatorChar
 * @param hasBracket
 * @return
 */
int priorityOfOperation(char operatorChar, bool inStack)
{
    if(!inStack) {
        //栈外优先级
        switch (operatorChar) {
            case '#':
                return 0;
            case '(':
                return 6;
            case ')':
                return 1;
            case '-':
            case '+':
                return 2;
            case '*':
            case '/':
                return 4;
            default:
                return -1;
        }
    } else {
        //栈内优先级
        switch (operatorChar) {
            case '#':
                return 0;
            case '(':
                return 1;
            case ')':
                return 6;
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
enum Type {VAR, OPERATOR, SPECIAL};
struct Token {
    string data;
    Type type;

    Token(string d, Type t):data(std::move(d)), type(t) {}

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
void splitString(const string & expression, const char & splitter, vector<Token *> & stringAfterSplit) {

    string newString;
    Token * p;

    for(char i : expression) {
        if(i != splitter) {
            newString.insert(newString.end(), i);
            if(isOperator(i))
                p = new Token(newString, OPERATOR);
            else
                p = new Token(newString, VAR);

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
    vector<Token *> tokens;

    //结果字符串
    string suffixExpression;

    //是否在栈中的标示
    bool inStack = false;

    //首先把#压入栈中，用于标识
    operationStack.push('#');

    //对表达式进行分词，按空格分割
    splitString(expression, ' ', tokens);

    //对结尾添加'#'
    tokens.push_back(new Token("#", SPECIAL));

    for(Token* & token: tokens) {

        if(token->type == VAR) {
            //如果token是变量，则直接添加到结果字符串中
            suffixExpression.append(token->data);
            suffixExpression.append(" ");
        } else if (token->type == OPERATOR || token->type == SPECIAL) {
            //如果是操作符，比较优先级
            if(priorityOfOperation(token->data[0], false) > priorityOfOperation(operationStack.top(), true)) {
                //把识别的符号和栈中的符号比较优先级，如果栈外优先级高于栈内优先级，则进栈
                operationStack.push(token->data[0]);
            } else {
                //如果栈外优先级低于栈内优先级，则把栈内的元素弹出，直到栈外元素高于栈顶元素
                while (priorityOfOperation(token->data[0], false) < priorityOfOperation(operationStack.top(), true)) {
                    //把符号加入到结果字符串
                    suffixExpression.append(string(1, operationStack.top()));
                    suffixExpression.append(" ");
                    //弹出
                    operationStack.pop();
                }
                //当循环结束以后，可能会出现两种情况，1、栈内优先级和栈外优先级相等 2、栈内优先级低于栈外优先级
                //如果两者优先级相等
                //这种情况只会发生在 1、左括号匹配到了右括号 2、表达式结束
                //如果当前符号是右括号或者#，那么直接把栈顶弹出即可
                if (token->data[0] == ')' || token->data[0] == '#')
                    operationStack.pop();

                    //如果不是相等，那么就是栈内优先级低于栈外优先级，则把当前符号进栈
                else {
                    operationStack.push(token->data[0]);
                }
            }

        }

    }

    return suffixExpression;

}


#endif //DATA_STRUCTURE_CPP_EXPRESSION_H
