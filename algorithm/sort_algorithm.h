//
// Created by 胡凌瑞 on 2020/7/21.
//

#ifndef DATA_STRUCTURE_CPP_SORT_ALGORITHM_H
#define DATA_STRUCTURE_CPP_SORT_ALGORITHM_H

#include <vector>
#include <iostream>
#include <algorithm>

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
void selectionSort(vector<int> & nums)
{
    int i, j;
    int minIndex;
    for(i = 0; i < nums.size(); ++i)
    {
        minIndex = i;
        //找到最小值的index
        for(j = i + 1; j < nums.size(); ++j) {
            if(nums[j] < nums[minIndex])
                minIndex = j;
        }

        std::swap(nums[i], nums[minIndex]);
    }
}

//冒泡排序
void bubbleSort(vector<int> & nums)
{
    unsigned long i, j;
    for(i = 0; i < nums.size(); ++i)
    {
        for(j = nums.size() - 1; j > i; --j)
        {
            if(nums[j-1] > nums[j])
                std::swap(nums[j], nums[j-1]);
        }
    }
}


//希尔排序
void shellSort(vector<int> & nums)
{
    
}



//堆排序

/**
 * 简单的排序不需要过多说明
 * 但是nlogn的排序需要好好思考一下
 */



#endif //DATA_STRUCTURE_CPP_SORT_ALGORITHM_H
