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

//这是通过栈实现的
string reversePolishNotation(const string & expression)
{
    stack<char> operationStack;
    vector<Token *> tokens;

    //结果字符串
    string suffixExpression;

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

struct ASTNode {

    Token *token;
    ASTNode *left;
    ASTNode *right;

    explicit ASTNode(Token * t, ASTNode * l = nullptr, ASTNode * r = nullptr) : token(t), left(l), right(r) {}

};

void clearAST(ASTNode * root)
{
    if(root != nullptr)
    {
        clearAST(root->left);
        clearAST(root->right);
        delete root;
        root = nullptr;
    }
}

void printReversePolish(ASTNode * root) {

    if(root != nullptr) {
        printReversePolish(root->left);
        printReversePolish(root->right);
        std::cout<< root->token->data << " ";
    }

}

//我们假设表达式都是以字母的形式输入的
ASTNode * buildAST(const string & express)
{

    vector<Token *> tokens;
    //用于存放操作符的栈
    stack<ASTNode *> operatorStack;
    //用于存放标识符的栈
    stack<ASTNode *> idStack;

    operatorStack.push(new ASTNode(new Token("#", SPECIAL)));

    //分词，可以看作一个简单的词法分析过程
    splitString(express, ' ', tokens);

    //初始化抽象语法树
    ASTNode * root;

    tokens.push_back(new Token("#", SPECIAL));

    //开始构建一个抽象语法树
    for(auto & token: tokens) {
        if(token->type == VAR) {
            //如果token是一个变量，则加入到idStack中
            auto newNode = new ASTNode(token);
            idStack.push(newNode);
        } else if (token->type == OPERATOR || token->type == SPECIAL) {
            //如果token是一个操作符，则需要进行比较优先级和合并树的操作
            //如果是操作符，比较优先级
            if(priorityOfOperation(token->data[0], false) > priorityOfOperation(operatorStack.top()->token->data[0], true)) {
                //把识别的符号和栈中的符号比较优先级，如果栈外优先级高于栈内优先级，则进栈
                operatorStack.push(new ASTNode(token));

            } else {
                //如果栈外优先级低于栈内优先级，则把栈内的元素弹出，直到栈外元素高于栈顶元素
                while (priorityOfOperation(token->data[0], false) < priorityOfOperation(operatorStack.top()->token->data[0], true)) {
                    //把符号节点作为根，两个标识符为子拼接
                    ASTNode * & parent = operatorStack.top();
                    operatorStack.pop();

                    ASTNode * & right = idStack.top();
                    idStack.pop();

                    ASTNode * & left = idStack.top();
                    idStack.pop();

                    parent->left = left;
                    parent->right = right;

                    //把新生成的节点入栈
                    idStack.push(parent);

                }
                //当循环结束以后，可能会出现两种情况，1、栈内优先级和栈外优先级相等 2、栈内优先级低于栈外优先级
                //如果两者优先级相等
                //这种情况只会发生在 1、左括号匹配到了右括号 2、表达式结束
                //如果当前符号是右括号或者#，那么直接把栈顶弹出即可
                if (token->data[0] == ')' || token->data[0] == '#')
                    operatorStack.pop();

                    //如果不是相等，那么就是栈内优先级低于栈外优先级，则把当前符号进栈
                else {
                    operatorStack.push(new ASTNode(token));
                }
            }
        }

    }

    //最后把栈中的树弹出
    ASTNode * result = idStack.top();
    idStack.pop();

    return result;

}


/**
 * 根据表达式树生成中缀表达式
 */

void convert(ASTNode* & root, int deep)
{
    if(root == nullptr)
        return;
    else if(root->left == nullptr && root->right == nullptr)
        std::cout<<root->token->data;
    else {
        if(deep > 1)
            std::cout<< "(";
        convert(root->left, deep + 1);
        std::cout<<root->token->data;
        convert(root->right, deep + 1);
        if(deep > 1)
            std::cout<< ")";
    }

}

void convertAstToInOrder(ASTNode* & root)
{
    convert(root, 1);
}

#endif //DATA_STRUCTURE_CPP_EXPRESSION_H
