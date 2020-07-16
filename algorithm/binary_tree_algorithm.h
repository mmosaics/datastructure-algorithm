//
// Created by 胡凌瑞 on 2020/7/15.
//

#ifndef DATA_STRUCTURE_CPP_BINARY_TREE_ALGORITHM_H
#define DATA_STRUCTURE_CPP_BINARY_TREE_ALGORITHM_H

#include <vector>
#include <algorithm>
#include <queue>

using std::queue;

/**
 * 这是一个实现二叉树算法的文件，所以没有通过类的方式，直接以面向过程的方法写算法
 */


//一些基本的辅助操作
struct BinaryNode {
    int element;
    BinaryNode *left;
    BinaryNode *right;

    explicit BinaryNode(const int &x, BinaryNode *lt = nullptr, BinaryNode *rt = nullptr)
            : element(x), left(lt), right(rt) {}

    explicit BinaryNode(int &&x, BinaryNode *lt = nullptr, BinaryNode *rt = nullptr)
            : element(x), left(lt), right(rt) {}

};

void insert(int x, BinaryNode *&root) {
    if (root == nullptr)
        root = new BinaryNode(x, nullptr, nullptr);
    else if (x < root->element)
        insert(x, root->left);
    else if (root->element < x)
        insert(x, root->right);
    else; // 如果相等，什么都不做，暂时不考虑值相等
}

void printTree(BinaryNode * root)
{
    if(root)
    {
        printTree(root->left);
        std::cout<<root->element << " ";
        printTree(root->right);
    }
}

//下面是具体的一些相关算法

/**
 * 设一颗二叉树中各节点的值互不相同，其先序遍历和中序遍历分别存于两个一维数组A[1...n]和B[1...n]中，试编写算法建立该二叉树的二叉链表
 */
/**
 * 主例程，由于是做练习，就直接给出主例程，不写驱动程序了
 * 分析：
 * @param A 先序遍历数组
 * @param B 中序遍历数组
 * @param al A数组左边起点
 * @param ar A数组右边终点
 * @param bl B数组左边起点
 * @param br B数组右边终点
 * @return
 */
BinaryNode *preInTree(int A[], int B[], int al, int ar, int bl, int br) {
    //首先找到根节点
    //根节点就是先序数组的第一个节点，所以我们可以直接获得
    int rootElement = A[al];

    //创建根节点
    auto root = new BinaryNode(rootElement);

    //然后我们需要划分数组，以左子树的数组和右子树的数组划分
    //我们知道了根节点之后，可以通过搜索中序数组的根节点的位置来对其划分，在中序遍历中，根节点左边的为左子树，根节点右边的为右子树
    //其中leftNum为左子树节点的个数，i为根节点在中序遍历中的序号
    int leftNum = 0;
    int i;
    for (i = bl; B[i] != rootElement; ++i, ++leftNum);

    //通过计算可以得知
    //在前序遍历数组A中，左子树的序列为 [al + 1, al + leftNum]，右子树的序列 [al + leftNum + 1, ar]
    //在中序遍历数组B中, 左子树的序列为 [bl, bl + leftNum - 1], 右子树的序列 [i + 1, br]
    //我们可以让左儿子等于左数组生成的根节点，右儿子等于右数组生成的根节点

    //所以最重要的就是边界判断，什么时候终止
    //我们之前算到了左子树的长度，这里给出一个右子树长度的计算公式，总节点数-左子树长度-根节点 = br - bl + 1 - leftNum - 1
    int rightNum = br - bl - leftNum;

    //这是左子树
    if (leftNum)
        root->left = preInTree(A, B, al + 1, al + leftNum, bl, bl + leftNum - 1);
    else
        root->left = nullptr;

    //这是右子树
    if (rightNum)
        root->right = preInTree(A, B, al + leftNum + 1, ar, i + 1, br);
    else
        root->right = nullptr;

    return root;


}


/**
 * 二叉树按二叉链表形式存储，写一个判别给定二叉树是否是完全二叉树的算法
 */
/**
 * 分析：这道题主要是要把握完全二叉树的结构特点，就是一定是按层数顺序排列的，所以可以很自然想到层序遍历
 * 具体想法是，
 * 1、让二叉树进行层序遍历，记录每次加入队列的节点数，如果一个树不是完全二叉树，则一定在某一层，从左到右缺了某几个节点
 * 2、我们进行层序遍历的过程中，会把节点的左儿子和右儿子加入队列，如果在某一次队列中只加入了一个节点，那么说明出现了一次空缺
 * 3、如果树是一颗完全二叉树，那么在之后就不应该会有新的节点进入队列，不然就会破坏完全二叉树的性质
 * 4、所以只需要在之后的步骤中检测是否有一次入队，如果有，则说明树不是完全二叉树
 * @param root
 * @return
 */
bool isCompleteTree(BinaryNode * root)
{

    queue<BinaryNode *> nodeQueue;

    //sentinel用于记录每次加入队列的元素的个数，为0、1、2
    int sentinel = 0;
    //flag记录是否有某一次入队数为1，如果有一次入队树为1，则flag为1，否则保持0
    int flag = 0;

    //把根节点加入队列中
    nodeQueue.push(root);

    //队列非空
    while(!nodeQueue.empty())
    {
        //将元素弹出
        BinaryNode * & p = nodeQueue.front();
        nodeQueue.pop();

        if(p->left) {
            nodeQueue.push(p->left);
            ++sentinel;
        }

        if(p->right) {
            nodeQueue.push(p->right);
            ++sentinel;
        }

        if(sentinel == 1 && flag == 0)
            flag = 1;

        if(sentinel > 0 && flag == 1)
            return false;

        sentinel = 0;

    }

    return true;

}


/**
 * 假设二叉查找树采用二叉链表存储结构存储，试设计一个算法，计算一棵给定二叉树的所有双分支节点个数
 */
/**
 * 也是考察的层序遍历
 */
int statisticTwoDegreeNode(BinaryNode * root)
{
    //需要一个队列辅助
    queue<BinaryNode *> nodeQueue;

    //统计分支数
    int degree = 0;

    //统计二分支节点数
    int num = 0;

    //将根节点进入队列
    nodeQueue.push(root);

    //如果队列不为空
    while(!nodeQueue.empty())
    {
        //出队
        BinaryNode * p = nodeQueue.front();
        nodeQueue.pop();

        if(p->left)
        {
            nodeQueue.push(p->left);
            ++degree;
        }
        if(p->right)
        {
            nodeQueue.push(p->right);
            ++degree;
        }

        if(degree == 2)
            ++num;

        degree = 0;

    }

    return num;

}


/**
 * 设树B是一棵采用二叉链表结构存储的二叉树，编写一个把树B中所有节点的左、右子树进行交换的函数
 */
/**
 * 这一道题比较简单，就不做过多解释，就是一个后序遍历的改版
 */
void reverseChild(BinaryNode*&);

void reverse(BinaryNode * root)
{
    if(root)
    {
        reverse(root->left);
        reverse(root->right);
        reverseChild(root);
    }

}

void reverseChild(BinaryNode * & node)
{
    BinaryNode * leftChild = node->left;
    node->left = node->right;
    node->right = leftChild;
}


/**
 * 假设二叉树采用二叉链存储结构存储，设计一个算法，求先序遍历序列中第k个节点的值
 */
/**
 * 想法是，执行先序遍历，然后遍历一个节点就把节点入队，最后依次出队并统计个数
 */

void preOrder(BinaryNode *, queue<BinaryNode *> &);

int findKth(BinaryNode * root, int k)
{
    queue<BinaryNode *> nodeQueue;

    preOrder(root, nodeQueue);

    BinaryNode * resultNode = nullptr;

    //计数器
    int count = 0;

    while (!nodeQueue.empty())
    {
        resultNode = nodeQueue.front();
        nodeQueue.pop();
        ++count;
        if(count == k)
            break;
    }

    return resultNode->element;
}


void preOrder(BinaryNode * root, queue<BinaryNode *> & nodeQueue)
{
    if(root)
    {
        nodeQueue.push(root);
        preOrder(root->left, nodeQueue);
        preOrder(root->right, nodeQueue);
    }
}



/**
 * 已知二叉树以二叉链表存储，编写算法完成；对于树中每个元素值为x的节点，删去以它为根的子树，并释放相应的空间
 */
/*
 * 本质也是一道先序遍历的题目，比较简单
 */

void clearTree(BinaryNode * & );


void deleteSubTreeOfX(BinaryNode * & root, int x)
{
    if(root)
    {
        //如果找到了对应元素，则删除该子树，并返回
        if(root->element == x) {
            clearTree(root);
            return;
        }
        deleteSubTreeOfX(root->left, x);
        deleteSubTreeOfX(root->right, x);
    }

}

void clearTree(BinaryNode * & root)
{
    if(root)
    {
        clearTree(root->left);
        clearTree(root->right);
        delete root;
        root = nullptr;
    }
}

/**
 * 下面给出非递归遍历树的方式
 */



/**
 * 在二叉树中查找值为x的节点，试编写算法打印值为x的节点的所有祖先，假设值为x的节点不多于1个
 */



#endif //DATA_STRUCTURE_CPP_BINARY_TREE_ALGORITHM_H
