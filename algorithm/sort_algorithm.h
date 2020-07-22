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

template <typename Object>
void printVector(vector<Object> & nums)
{
    for(auto & num : nums)
        std::cout<<num<<std::endl;
        //std::cout<< num << " ";

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


//归并排序

/**
 * 归并排序核一共有两个核心步骤
 * 一个是分，从中间开始分，划分为两个部分
 * 一个是合，当左半部分和右半部分都已经有序的时候，我们需要用双指针法把他们合并到同一个数组当中
 */


//归并排序的合并例程
void merge(vector<int> & nums, vector<int> & tmp, int left, int mid, int right)
{
    //右半部分的开始下标
    int rightStart = mid + 1;
    //两个指针分别指向两个数组
    int i = left, j = rightStart;
    int tmpPos = left;

    while(i <= mid && j <= right)
    {
        if(nums[i] < nums[j])
            tmp[tmpPos++] = nums[i++];
        else
            tmp[tmpPos++] = nums[j++];
    }

    while (i <= mid)
        tmp[tmpPos++] = nums[i++];

    while (j <= right)
        tmp[tmpPos++] = nums[j++];

    //此时tmp数组已经是排序好的数组，只需要把tmp数组复制到nums当中即可
    for(int k = right; k >= left; --k)
        nums[k] = tmp[k];
}

//归并排序主程序
void mergeSort(vector<int> & nums, vector<int> & tmp, int left, int right)
{
    if(left < right)
    {
        int mid = (right-left)/2 + left;
        //对左边进行归并排序
        mergeSort(nums, tmp, left, mid);
        mergeSort(nums, tmp, mid+1, right);
        merge(nums, tmp, left, mid, right);
    }
}

//归并排序驱动程序
void mergeSort(vector<int> & nums)
{
    vector<int> tmp(nums.size());
    mergeSort(nums, tmp, 0, static_cast<int>(nums.size()-1));
}

/*
 * 总结一下，在写递归程序的时候，除非特殊情况，一定要排除字面值（即直接写0，1这种数字）
 * 至少在归并程序中，必须把每一个问题当作子问题看待，而其中tmp数组就不能随意设置，值存放的位置，应该与nums数组的位置相关
 * 比如最开始我把tmpPos初始化为0，那么每次子问题归并的时候，都会从tmp数组的0开始合并，那么这样会把已经归并好的数组给覆盖掉
 * 因此正确的做法应该是把tmpPos设置为left，和nums数组的位置保持一致
 */


//快速排序

/**
 * 快速排序，核心是pivot，每次找到一个pivot然后把pivot放到正确的位置上
 */

void insertionSort(vector<int> & nums, int left, int right) {
    int waitForInsertion;
    int i, j;
    for (i = left + 1; i < right; ++i) {
        waitForInsertion = nums[i];
        for (j = i; j > 0 && nums[j - 1] > waitForInsertion; --j)
            nums[j] = nums[j - 1];
        nums[j] = waitForInsertion;
    }
}

int median3(vector<int> & nums, int left, int right)
{
    int mid = (right-left)/2 + left;
    if(nums[left] > nums[right])
        std::swap(nums[left], nums[right]);
    if(nums[left] > nums[mid])
        std::swap(nums[left], nums[mid]);
    if(nums[mid] > nums[right])
        std::swap(nums[mid], nums[right]);

    std::swap(nums[right-1], nums[mid]);
    return nums[right-1];
}

void quickSort(vector<int> & nums, int left, int right)
{
    /*
    if(right - left + 1 <= 3)
    {
        int mid = (right-left)/2 + left;
        if(nums[left] > nums[right])
            std::swap(nums[left], nums[right]);
        if(nums[left] > nums[mid])
            std::swap(nums[left], nums[mid]);
        if(nums[mid] > nums[right])
            std::swap(nums[mid], nums[right]);
    }
     */
    if(right - left > 10)
    {
        int pivot = median3(nums, left, right);
        //i为左指针，j为右指针
        int i = left-1, j = right-1;
        for(;;)
        {
            //如果i小于pivot会一直走，直到遇到比pivot大的停下
            while (nums[++i] < pivot);
            //如果j大于pivot会一直走，直到遇到比pivot小的停下
            while (nums[--j] > pivot);
            //交换两者位置
            if(i < j)
                std::swap(nums[i], nums[j]);
            else
                break;
        }

        //上面循环结束后，说明i已经到了j的右边，这个时候交换i和pivot
        std::swap(nums[i], nums[right-1]);
        //然后对i左边的和i右边的再次进行快排
        quickSort(nums, left, i-1);
        quickSort(nums, i+1, right);
    }

    insertionSort(nums, left, right);
}

void quickSort(vector<int> & nums)
{
    quickSort(nums, 0, nums.size()-1);
}

//基数排序

/**
 * 基数排序是一种比较特殊的办法，具体就不过多介绍，主要用于字符串的排序
 */

void radixSort(vector<string> & words, int maxLen)
{
    const int BUCKET = 256;

    //加一的原因是我们需要直接用长度来索引，如果仅仅是maxLen，那么最大长度就需要用maxLen-1，会很不方便，且无法处理空字符
    vector<vector<string>> wordsByLength(maxLen+1);
    vector<vector<string>> bucket(BUCKET);

    //先按照长度排好序
    for(auto & str: words)
        wordsByLength[str.length()].push_back(str);

    int idx = 0;
    for(auto & wordList: wordsByLength) {
        for (auto &word: wordList)
            words[idx++] = std::move(word);
    }

    int startingIndex = words.size();
    //要从最后一个字母开始基数排序，而最后一个字母的起始位置应该是maxLen-1；
    for(int pos = maxLen - 1; pos >= 0; --pos) {

        //计算这个长度的字符的开始位置，就是用总的size 减去这个长度的单词的size，我们可以通过wordsByLength获取到
        startingIndex -= wordsByLength[pos+1].size();

        //把单词放进桶中
        for(int i = startingIndex; i < words.size(); ++i)
            bucket[words[i][pos]].push_back(words[i]);

        idx = startingIndex;
        //然后再取出
        for(auto & theBucket: bucket) {
            for(auto & word: theBucket)
                words[idx++] = word;
        }
        bucket.clear();
    }

}

#endif //DATA_STRUCTURE_CPP_SORT_ALGORITHM_H