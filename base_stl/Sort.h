//
// Created by 胡凌瑞 on 2020/7/6.
//

#ifndef DATA_STRUCTURE_CPP_SORT_H
#define DATA_STRUCTURE_CPP_SORT_H

#include <vector>
#include <iostream>

using std::vector;
using std::string;

template <typename Comparable>
void insertionSort(vector<Comparable> & a)
{
    /*
    for(int p = 1; p < a.size(); ++p)
    {
        Comparable tmp = std::move(a[p]);

        int j;
        for( j = p; j > 0 && tmp < a[j-1]; --j)
            a[j] = std::move(a[j-1]);
        a[j] = std::move(tmp);
    }
     */
    insertionSort(a, 0, a.size() - 1);
}

//start = 0, end = size()-1
template <typename Comparable>
void insertionSort(vector<Comparable> & a, int start, int end)
{
    for(int p = start + 1; p <= end; ++p)
    {
        Comparable tmp = std::move(a[p]);

        int j;
        for( j = p; j > start && tmp < a[j-1]; --j)
            a[j] = std::move(a[j-1]);
        a[j] = std::move(tmp);
    }
}

template <typename Iterator, typename Comparator>
void insertionSort( const Iterator & begin, const Iterator & end, Comparator lessThan)
{
    if(begin == end)
        return;

    Iterator j;
    for( Iterator p = begin+1; p != end; ++p)
    {
        auto tmp = std::move(*p);
        for(j = p; j != begin && lessThan(tmp, *(j-1)); --j)
            *j = std::move(*(j-1));
        *j = std::move(tmp);
    }
}

template <typename Iterator>
void insertionSort( const Iterator & begin, const Iterator & end)
{
    insertionSort(begin, end, std::less<decltype(*begin)>{} );
}

template <typename Comparable>
void bubbleSort(vector<Comparable> & a)
{
    for(int i = 0; i < a.size(); ++i)
        for(int j = a.size()-1; j > i; --j)
        {
            if(a[j] < a[j-1])
                std::swap(a[j], a[j-1]);
        }
}

template <typename Comparable>
void selectionSort(vector<Comparable> & a)
{
    int minIndex;
    for(int i = 0; i < a.size(); ++i) {
        minIndex = i;
        for (int j = i+1; j < a.size(); ++j) {
            if(a[j] < a[minIndex])
                minIndex = j;
        }
        std::swap(a[i], a[minIndex]);
    }
}

template <typename Comparable>
void shellSort(vector<Comparable> & a)
{
    for(int gap = a.size()/2; gap>0; gap /= 2)
    {
        for(int i = gap; i < a.size(); ++i)
        {
            Comparable tmp = std::move(a[i]);
            int j;
            for(j = i; j >= gap  && tmp < a[j-gap]; j -= gap)
                a[j] = std::move(a[j-gap]);
            a[j] = std::move(tmp);

        }
    }
}

//堆排序


inline int leftChild(int i)
{
    return i * 2 + 1;
}

/**
 * 下滤
 * @tparam Comparable
 * @param a
 * @param i
 * @param n
 */
template <typename Comparable>
void percolateDown(vector<Comparable> & a, int i, int n)
{
    int child;
    Comparable tmp;
    for(tmp = std::move(a[i]); leftChild(i) < n; i = child)
    {
        child = leftChild(i);
        if(child != n - 1 && a[child + 1] > a[child])
            child++;
        if(tmp < a[child])
            a[i] = std::move(a[child]);
        else
            break;
    }
    a[i] = std::move(tmp);

}

template <typename Comparable>
void heapSort(vector<Comparable> & a)
{
    //构建堆
    for(int i = a.size() / 2 - 1; i >= 0; --i )
        percolateDown(a, i, a.size());

    //执行deleteMax操作，并把Max放在尾部
    for(int j = a.size() - 1; j > 0; --j)
    {
        std::swap(a[0], a[j]);
        percolateDown(a, 0, j);
    }

}


//归并排序


template <typename Comparable>
void mergeSort(vector<Comparable> & a)
{
    vector<Comparable> tmpArray(a.size());
    mergeSort(a, tmpArray, 0, a.size()-1);
}


/**
 * 归并排序主程序
 * @tparam Comparable
 * @param a
 * @param tmpArray
 * @param left
 * @param right
 */
template <typename Comparable>
void mergeSort(vector<Comparable> & a, vector<Comparable> & tmpArray,
                int left, int right)
{
    if(left < right)
    {
        int center = (left + right) / 2;
        mergeSort(a, tmpArray, left, center);
        mergeSort(a, tmpArray, center+1, right);
        merge(a, tmpArray, left, center + 1, right);
    }
}

/**
 * 合并两个数组
 * a 为数组
 * tmpArray用于保存归并结果
 * leftPos子数组最左元素下标
 * rightPos为后半部分起点的下标
 * rightEnd为最右元素的下标
 */
template <typename Comparable>
void merge(vector<Comparable> & a, vector<Comparable> & tmpArray,
            int leftPos, int rightPos, int rightEnd)
{
    int tmp = leftPos;
    int leftEnd = rightPos - 1;
    int nums = rightEnd - leftPos + 1;
    while(leftPos <= leftEnd && rightPos <= rightEnd )
        if(a[leftPos] < a[rightPos])
            tmpArray[tmp++] = std::move(a[leftPos++]);
        else
            tmpArray[tmp++] = std::move(a[rightPos++]);

    //处理剩下的元素
    while (leftPos <= leftEnd)
        tmpArray[tmp++] = std::move(a[leftPos++]);

    while (rightPos <= rightEnd)
        tmpArray[tmp++] = std::move(a[rightPos++]);

    //把tmpArray里面的元素复制到a里面
    for(int i = 0; i < nums; ++i, --rightEnd)
        a[rightEnd] = std::move(tmpArray[rightEnd]);

}




//快速排序


/**
 * 快速排序驱动程序
 * @tparam Comparable
 * @param a
 */
template <typename Comparable>
void quickSort(vector<Comparable> & a)
{
    quickSort(a, 0, a.size() - 1);
}

/**
 * 利用三中值法求pivot
 * @tparam Comparable
 * @param a
 * @param left
 * @param right
 * @return
 */
template <typename Comparable>
const Comparable & median3(vector<Comparable> & a, int left, int right)
{
    int center = (left + right)/2;

    //让left，right，center放到各自正确的位置上
    if(a[right] < a[left])
        std::swap(a[left], a[right]);
    if(a[right] < a[center])
        std::swap(a[center], a[right]);
    if(a[center] < a[left])
        std::swap(a[center], a[left]);

    std::swap(a[center], a[right-1]);
    return a[right - 1];
}


/**
 * 快速排序主程序
 * @tparam Comparable
 * @param a
 * @param left
 * @param right
 */
template <typename Comparable>
void quickSort(vector<Comparable> & a, int left, int right )
{
    if(left + 10 <= right)
    {
        const Comparable & pivot = median3(a, left, right);

        int i = left, j = right - 1;
        for(;;)
        {
            //移动i，j到正确的位置
            //i应该在比pivot大的地方停下，j应该在比pivot小的地方停下
            while (a[++i] < pivot);
            while (a[--j] > pivot);

            if(i < j)
                std::swap(a[i], a[j]);
            else
                break;
        }

        //交换i和pivot,此时pivot就已经在正确的位置上
        std::swap(a[i], a[right - 1]);

        quickSort(a, left, i - 1);
        quickSort(a, i+1, right);

    }
    else
        insertionSort(a, left, right);

}


//基数排序
/**
 * 对字符串进行基数排序
 * @param arr
 * @param maxLen
 */
void radixSort(vector<string> & arr, int maxLen) {
    const int BUCKETS = 256;

    vector<vector<string>> wordsByLength(maxLen + 1);
    vector<vector<string>> buckets(BUCKETS);

    for (string &s : arr)
        wordsByLength[s.length()].push_back(std::move(s));

    int idx = 0;
    for (auto &wordList: wordsByLength)
        for (string &s: wordList)
            arr[idx++] = std::move(s);

    int startingIndex = arr.size();

    for( int pos = maxLen - 1; pos >= 0; --pos)
    {
        //wordByLength[pos + 1].size() 是获得对应长度的单词的个数
        //startingIndex是求出有相同长度单词的开始位置
        startingIndex -= wordsByLength[pos + 1].size();

        for(int i = startingIndex; i < arr.size(); ++i)
            buckets[arr[i][pos]].push_back(std::move(arr[i]));

        idx = startingIndex;
        for(auto & thisBucket: buckets)
        {
            for(string & s: thisBucket)
                arr[idx++] = std::move(s);

            thisBucket.clear();
        }

    }

}





#endif //DATA_STRUCTURE_CPP_SORT_H
