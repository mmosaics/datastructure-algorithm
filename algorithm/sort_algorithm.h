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

/**
 * 简单的排序不需要过多说明
 * 但是nlogn的排序需要好好思考一下
 */

/**
 * 希尔排序就是一种优化了的插入排序，只是对于元素的处理是间隔了一定的距离
 * @param nums
 */
void shellSort(vector<int> & nums)
{
    for(unsigned int gap = nums.size()/2; gap > 0; gap /= 2)
    {
        for(unsigned int i = gap; i < nums.size(); ++i)
        {
            int waitForInsert = nums[i];
            unsigned int j;
            for(j = i; j >= gap && nums[j - gap] > waitForInsert; j -= gap)
                nums[j] = nums[j - gap];
            nums[j] = waitForInsert;
        }
    }
}

//堆排序

/**
 * 堆排序需要有一个建堆的过程
 * 为了节省空间，不一定需要专门使用一个堆来实现，我们可以直接在数组上建堆，然后一直pop，就可以完成排序
 * 综上，再结合数组的特点，我们可以建立一个最大堆，然后每次findMax，把最大值放到尾部即可
 * 然后，对于一般的二叉堆来说，数组的0位置应该是保留的，对于一个待排序的数组，一般不具有这样的特性，所以需要区分一下
 */

inline int leftChild(int i)
{
    return i * 2 + 1;
}

void percolateDown(vector<int> & nums, int i, int n)
{
    int child;
    int tmp;
    for(tmp = nums[i]; leftChild(i) < n; i = child)
    {
        child = leftChild(i);
        if(child != n-1 && nums[child] < nums[child+1])
            ++child;
        if(tmp < nums[child])
            nums[i] = nums[child];
        else
            break;
    }
    nums[i] = tmp;
}

void heapSort(vector<int> & nums)
{
    //首先构建堆
    for(int i = nums.size()/2 - 1; i >= 0; --i)
        percolateDown(nums, i, nums.size());

    for(int i = nums.size() - 1; i >= 0; --i)
    {
        int max = nums[0];
        int deleted = nums[i];

        //把最后一个元素放到最大值位置，并进行下滤，这个时候等于在堆中删除一个元素，所以下滤的范围也要更改
        nums[0] = deleted;
        percolateDown(nums, 0, i);

        //下滤完成之后，把最大值放到末尾
        nums[i] = max;
    }

}



#endif //DATA_STRUCTURE_CPP_SORT_ALGORITHM_H
