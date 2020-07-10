//
// Created by 胡凌瑞 on 2020/7/10.
//

#ifndef DATA_STRUCTURE_CPP_PATTERN_H
#define DATA_STRUCTURE_CPP_PATTERN_H

#include <string>
#include <vector>

using std::string;
using std::vector;

/**
 * 这是一个关于模式匹配的算法的实践，当然不仅仅局限于模式匹配，主要是关于字符串的一些算法
 * 从最开始的暴力搜索到kmp，慢慢实现
 */


//Brute-Force
/**
 * 最普通的字符串匹配，暴力匹配，时间复杂度过高
 * @param p 需要搜索的模式
 * @param t 目标字符串
 * @return 匹配的模式串在目标中第一次出现的开始位置
 */
int normalSearch(string p, string t)
{
    int i, j;
    int matchLen = 0;
    //直接初始化，从0开始搜索
    for(i = 0, j = 0;  i < p.length() && j < t.length(); )
    {
        if(p[i] == t[j]) {
            //如果一样，则继续搜索
            ++i; ++j;
            matchLen++;
            if(matchLen == p.length()) {
                j = j - i;
                break;
            }
        } else {
            //如果不同，则回退到 被匹配字符串开始位置的下一个位置， 然后p又从0开始
            j = j - i + 1;
            i = 0;
            matchLen = 0;
        }

    }

    //如果没有匹配到返回 -1
    if(matchLen == 0)
        return -1;

    return j;

}




//KMP
//KMP最重要的两个部分，一是生成next数组，另一个就是主例程
/**
 * next数组的核心概念其实应该是PMT（Partial Match Table），而真实的next数组就是PMT数组往后移了一位，然后next[0]被置为-1
 * 如果直接求next数组，很容易陷入编程细节，在实现next数组的时候可以按照正常方式实现PMT数组，成功实现之后再通过修改坐标的方式把生成的数组改为next数组
 * @param p 模式字符串
 * @param next 需要生成的next数组
 */
void generateNextArray(const string & p, vector<int> & next)
{
    next.resize(p.length() + 1);
    next[0] = -1;
    next[1] = 0;
    //i用于指示对p字符串的遍历
    //j用于完成对字符串前缀的匹配
    int i = 1, j = 0;
    for(; i < p.length() && j < p.length(); ++i)
    {
        if(p[i] == p[j]) {
            ++j;
            next[i+1] = next[i] + 1;
        } else {
            j = 0;
            next[i+1] = j;
        }
    }

}

/**
 * kmp算法实现
 * @param p 模式字符串
 * @param t 目标字符串
 * @return 具有指定模式的字符串的首次出现位置,若没有指定模式，返回-1
 */
int kmp(const string & p, const string & t)
{
    vector<int> next;
    generateNextArray(p, next);
    //i用于遍历t(目标字符串)
    int i;
    //j用于遍历模式字符串
    int j;
    for(i = 0, j = 0; i < t.length() && j < p.length(); )
    {
        if( next[j] == -1 || t[i] == p[j]) {
            //相等则继续推进i和j
            if(j == p.length() -1) {
                //如果j已经达到了p的长度，说明已经匹配完成，把i移到开头，返回即可
                i = i - j;
                break;
            }
            ++i;
            ++j;
        } else {
            j = next[j];
        }

    }
    //没有匹配到字符串，返回-1
    if(i == t.length())
        i = -1;
    return i;
}

#endif //DATA_STRUCTURE_CPP_PATTERN_H
