//
// Created by 胡凌瑞 on 2020/7/21.
//

#ifndef DATA_STRUCTURE_CPP_SORT_ALGORITHM_H
#define DATA_STRUCTURE_CPP_SORT_ALGORITHM_H

#include <vector>

using std::vector;

/**
 * 实现了排序算法，这里主要使用vector的形式，对int类型数据排序
 * 为了实现简单，都默认从小到大排序
 */

//基本的排序算法
void insertionSort(vector<int> & nums)
{
    int waitForInsert;
    for(int i = 1; i < nums.size(); ++i)
    {
        waitForInsert = nums[i];
        for(int j = i - 1; j >= 0; --j)
        {
            

        }


    }


}





#endif //DATA_STRUCTURE_CPP_SORT_ALGORITHM_H
