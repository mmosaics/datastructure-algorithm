//
// Created by 胡凌瑞 on 2020/7/21.
//

#ifndef DATA_STRUCTURE_CPP_SORT_ALGORITHM_H
#define DATA_STRUCTURE_CPP_SORT_ALGORITHM_H

#include <vector>
#include <iostream>

using std::vector;

/**
 * 实现了排序算法，这里主要使用vector的形式，对int类型数据排序
 * 为了实现简单，都默认从小到大排序
 */

void printVector(vector<int> & nums)
{
    for(auto & num : nums)
        std::cout<< num << " ";

    std::cout<<std::endl;

}

//vector<int> nums{23, 45, 11, 90, 27, 20, 37, 78};

//排序算法
void insertionSort(vector<int> & nums)
{
    int waitForInsert;
    int i, j;
    for(i = 1; i < nums.size(); ++i)
    {
        waitForInsert = nums[i];
        for(j = i; j > 0 && waitForInsert < nums[j-1]; --j)
            nums[j] = nums[j-1];
        nums[j] = waitForInsert;
    }

}


//选择排序





#endif //DATA_STRUCTURE_CPP_SORT_ALGORITHM_H
