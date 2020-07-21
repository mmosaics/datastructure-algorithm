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

void printTree(BinaryNode *root) {
    if (root) {
        printTree(root->left);
        std::cout << root->element << " ";
        printTree(root->right);
    }
}

//下面是具体的一些相关算法

/**
 * 递归求高度
 */

int heightWithRecursive(BinaryNode *&root) {
    BinaryNode *p = root;
    if (!p)
        return 0;
    return std::max(heightWithRecursive(p->left), heightWithRecursive(p->right)) + 1;

}

/**
 * 使用非递归算法求二叉树高度
 */
int height(BinaryNode *&root) {
    queue<BinaryNode *> nodeQueue;

    BinaryNode *p = root;

    nodeQueue.push(p);

    BinaryNode *last = nodeQueue.back();

    int height = 0;

    while (!nodeQueue.empty()) {
        p = nodeQueue.front();
        nodeQueue.pop();

        if (p->left)
            nodeQueue.push(p->left);
        if (p->right)
            nodeQueue.push(p->right);

        if (p == last) {
            last = nodeQueue.back();
            height++;
        }

    }

    return height;

}

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
bool isCompleteTree(BinaryNode *root) {

    queue<BinaryNode *> nodeQueue;

    //sentinel用于记录每次加入队列的元素的个数，为0、1、2
    int sentinel = 0;
    //flag记录是否有某一次入队数为1，如果有一次入队树为1，则flag为1，否则保持0
    int flag = 0;

    //把根节点加入队列中
    nodeQueue.push(root);

    //队列非空
    while (!nodeQueue.empty()) {
        //将元素弹出
        BinaryNode *&p = nodeQueue.front();
        nodeQueue.pop();

        if (p->left) {
            nodeQueue.push(p->left);
            ++sentinel;
        }

        if (p->right) {
            nodeQueue.push(p->right);
            ++sentinel;
        }

        if (sentinel == 1 && flag == 0)
            flag = 1;

        if (sentinel > 0 && flag == 1)
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
int statisticTwoDegreeNode(BinaryNode *root) {
    //需要一个队列辅助
    queue<BinaryNode *> nodeQueue;

    //统计分支数
    int degree = 0;

    //统计二分支节点数
    int num = 0;

    //将根节点进入队列
    nodeQueue.push(root);

    //如果队列不为空
    while (!nodeQueue.empty()) {
        //出队
        BinaryNode *p = nodeQueue.front();
        nodeQueue.pop();

        if (p->left) {
            nodeQueue.push(p->left);
            ++degree;
        }
        if (p->right) {
            nodeQueue.push(p->right);
            ++degree;
        }

        if (degree == 2)
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
void reverseChild(BinaryNode *&);

void reverse(BinaryNode *root) {
    if (root) {
        reverse(root->left);
        reverse(root->right);
        reverseChild(root);
    }

}

void reverseChild(BinaryNode *&node) {
    BinaryNode *leftChild = node->left;
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

int findKth(BinaryNode *root, int k) {
    queue<BinaryNode *> nodeQueue;

    preOrder(root, nodeQueue);

    BinaryNode *resultNode = nullptr;

    //计数器
    int count = 0;

    while (!nodeQueue.empty()) {
        resultNode = nodeQueue.front();
        nodeQueue.pop();
        ++count;
        if (count == k)
            break;
    }

    return resultNode->element;
}


void preOrder(BinaryNode *root, queue<BinaryNode *> &nodeQueue) {
    if (root) {
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

void clearTree(BinaryNode *&);


void deleteSubTreeOfX(BinaryNode *&root, int x) {
    if (root) {
        //如果找到了对应元素，则删除该子树，并返回
        if (root->element == x) {
            clearTree(root);
            return;
        }
        deleteSubTreeOfX(root->left, x);
        deleteSubTreeOfX(root->right, x);
    }

}

void clearTree(BinaryNode *&root) {
    if (root) {
        clearTree(root->left);
        clearTree(root->right);
        delete root;
        root = nullptr;
    }
}

/**
 * 下面给出非递归遍历树的方式
 */

//中序遍历（非递归）
//递归的本质是栈，把一个程序改为非递归，肯定是需要栈做辅助的
void inOrderWithoutRecursive(BinaryNode *&root) {
    stack<BinaryNode *> nodeStack;
    BinaryNode *p = root;
    //如果p不为空或者栈不为空，则处理树节点
    while (p || !nodeStack.empty()) {
        //我们需要一直访问p的左儿子，一直到最深处，然后直到左儿子为空，会进入else例程，此时我们知道，栈顶就是p
        if (p) {
            nodeStack.push(p);
            p = p->left;
        }
            //如果为空，我们弹出栈顶元素，这个时候栈顶元素其实就是空的节点的父节点，那么我们只需要弹出，再访问，就达到了先访问左儿子，再父亲，然后把指针指向右儿子，就能完成中序的顺序
        else {
            p = nodeStack.top();
            nodeStack.pop();
            //visit(p);
            p = p->right;
        }
    }
}

//先序遍历（非递归）
void preOrderWithoutRecursive(BinaryNode *&root) {
    stack<BinaryNode *> nodeStack;
    BinaryNode *p = root;
    while (p || !nodeStack.empty()) {
        if (p) {
            //visit(p);
            nodeStack.push(p);
            p = p->left;
        } else {
            p = nodeStack.top();
            p = p->right;
        }
    }
}

//后序遍历
void postOrderWithoutRecursive(BinaryNode *&root) {
    stack<BinaryNode *> nodeStack;
    BinaryNode *p = root;
    BinaryNode *recent = nullptr;

    while (p || !nodeStack.empty()) {
        if (p) {
            nodeStack.push(p);
            p = p->left;
        }
            /*
             * 对于后序遍历，和前序遍历、中序遍历的情况不太一样
             * 我们在前序遍历和中序遍历中，其实控制流程是一模一样的，不一样的只是访问元素的时机，在这两种遍历中，如果遇到左儿子为空
             *  我们可以直接把栈中元素弹出，然后进行访问（这里主要是指中序，前序在入栈前就已经访问）
             *
             * 但是后序情况不太一样，我们在遇到左儿子为空的时候，不能立即弹出栈中元素，因为我们必须保证在访问左儿子之后的节点必须是右儿子
             * 一次弹出，就等于我们是对该节点的一次访问。因此我们必须得保证父节点在访问了左儿子和右儿子之后弹出
             * 因此在判断到空指针的时候，我们需要进行一定的条件判断，来判断是否存在右儿子，然后再进行相关的操作
             */
            //如果没有左儿子，对右儿子进行判断
        else {
            //这里我们取出栈顶元素，但是并不弹出，是为了获取到它的右儿子
            p = nodeStack.top();
            //之所以设置recent变量，是为了记录是否已经访问过右儿子，因为我们没有把元素弹出
            //如果不标示一个右儿子是否访问过，那么程序总是会返回到这个右儿子的父节点，然后进入无限循环
            if (p->right && p->right != recent) {
                //如果有右儿子，那么我们就把它按照正常的递归过程，仍然放入栈，然后进入新的循环
                p = p->right;
                nodeStack.push(p);
                p = p->left;
            }
                //这对应于没有右儿子的情况，这种情况我们就可以把它弹出
            else {
                nodeStack.pop();
                //visit(p);
                recent = p;
                p = nullptr;
            }
        }
    }
}

/**
 * 在二叉树中查找值为x的节点，试编写算法打印值为x的节点的所有祖先，假设值为x的节点不多于1个
 */
void findAncestorOfX(BinaryNode *&root, int x, vector<BinaryNode *> &ancestor) {
    stack<BinaryNode *> nodeStack;
    BinaryNode *p = root;
    BinaryNode *recent = nullptr;

    while (p || !nodeStack.empty()) {
        if (p) {
            if (p->element == x)
                break;
            nodeStack.push(p);
            p = p->left;
        } else {
            p = nodeStack.top();
            //有右儿子
            if (p->right && p->right != recent) {
                p = p->right;
                nodeStack.push(p);
                p = p->left;
            }
                //没有右儿子
            else {
                nodeStack.pop();
                recent = p;
                p = nullptr;
            }
        }
    }

    //只要栈非空，就一直弹出，弹出的都是x的祖先
    while (!nodeStack.empty()) {
        ancestor.push_back(nodeStack.top());
        nodeStack.pop();
    }

}

/**
 * 编写算法找到p和q的最近公共祖先节点r
 */
//可以通过使用上面一题已经写好的方法，然后通过两个祖先数组找到最近的元素即可
void ancestor(BinaryNode *&root, BinaryNode *p, BinaryNode *q, BinaryNode *&r) {
    vector<BinaryNode *> pAncestor;
    vector<BinaryNode *> qAncestor;

    findAncestorOfX(root, p->element, pAncestor);
    findAncestorOfX(root, q->element, qAncestor);

    BinaryNode *target;

    int flag = 0;

    for (auto &pNode: pAncestor) {
        for (auto &qNode: qAncestor) {
            if (pNode == qNode && flag == 0) {
                target = pNode;
                flag = 1;
            }
        }
    }

    r = target;

}

/**
 * 设计一个算法，求非空二叉树的宽短
 */

int breath(BinaryNode *&root) {
    queue<BinaryNode *> nodeQueue;

    BinaryNode *p = root;

    nodeQueue.push(p);

    BinaryNode *last = nodeQueue.back();

    int maxWidth = -1;
    int levelWidth = 0;

    while (!nodeQueue.empty()) {
        p = nodeQueue.front();
        nodeQueue.pop();
        ++levelWidth;

        if (p->left)
            nodeQueue.push(p->left);
        if (p->right)
            nodeQueue.push(p->right);

        if (levelWidth > maxWidth)
            maxWidth = levelWidth;

        if (last == p) {
            last = nodeQueue.back();
            levelWidth = 0;
        }
    }

    return maxWidth;

}

/**
 * 设有一棵满二叉树，已知其先序序列为pre，设计一个算法求其后序序列
 */

void buildTree(BinaryNode *&root, int pre[], int l, int r) {
    if (l > r)
        return;

    int total = r - l + 1;

    int halfTreeSize = (total - 1) / 2;

    int subLeftTreeLeft = l + 1;
    int subLeftTreeRight = l + halfTreeSize;

    int subRightTreeLeft = l + halfTreeSize + 1;
    int subRightTreeRight = l + 2 * halfTreeSize;

    root = new BinaryNode(pre[l]);

    //构建左子树
    buildTree(root->left, pre, subLeftTreeLeft, subLeftTreeRight);
    //构建右子树
    buildTree(root->right, pre, subRightTreeLeft, subRightTreeRight);

}

void postOrder(BinaryNode *&root, vector<int> &post) {
    if (root) {
        postOrder(root->left, post);
        postOrder(root->right, post);
        post.push_back(root->element);
    }
}

void getPostByPre(int pre[], vector<int> &post, int n) {
    BinaryNode *root;
    buildTree(root, pre, 0, n - 1);
    postOrder(root, post);

}

/**
 * 设计一个算法将二叉树的叶节点从左到右的顺序连成一个单链表，表头指针为head，二叉树按二叉链表方式存储，
 * 链接时用叶节点的右指针域来存在单链表指针
 */

void linkLeaf(BinaryNode *&root, BinaryNode *&pre, BinaryNode *&head) {
    if (root) {
        linkLeaf(root->left, pre, head);
        linkLeaf(root->right, pre, head);

        if (root->left == nullptr && root->right == nullptr && pre != nullptr) {
            pre->right = root;
            pre = root;
        }
        if (root->left == nullptr && root->right == nullptr && pre == nullptr) {
            pre = root;
            head = root;
        }
    }
}

BinaryNode *linkLeaf(BinaryNode *&root) {
    BinaryNode *pre = nullptr;
    BinaryNode *head = nullptr;
    linkLeaf(root, pre, head);
    return head;
}


/**
 * 试设计判断两棵树是否相似的算法，所谓二叉树T1和T2相似，指的是T1和T2都是空的二叉树或都只有一个根节点，或T1的左子树和T2的左子树是相似的
 * 且T1的右子树的T2的右子树是相似的
 */

int similar(BinaryNode *&t1, BinaryNode *&t2) {
    //这道题应该采用递归的思想，因为题目其实就已经暗示了递归的方法
    int left;
    int right;

    //如果两树皆为空，则相似
    if (t1 == nullptr && t2 == nullptr)
        return 1;

    //如果有一树为空，另一树不为空，则不相似
    if (t1 == nullptr || t2 == nullptr)
        return 0;
        //否则两树都不为空，则需要进一步判断左右子树的情况
    else {
        left = similar(t1->left, t2->left);
        right = similar(t1->right, t2->right);
        return left && right;
    }

}

/**
 * 二叉树的带权路径长度（WPL）是二叉树中所有叶节点的带权路径长度之和。给定一棵二叉树T，采用二叉链表存储
 * 请设计求T的WPL的算法
 */
//本质上还是可以通过层序遍历来做，遍历的过程记录高度，然后遇到叶子节点就计算带权路径长

int computeWPL(BinaryNode *&root) {
    queue<BinaryNode *> nodeQueue;

    BinaryNode *p = root;

    int WPL = 0;

    int length = 0;

    nodeQueue.push(p);

    BinaryNode *last = nodeQueue.front();


    while (!nodeQueue.empty()) {
        p = nodeQueue.front();
        nodeQueue.pop();

        int isLeaf = 0;

        if (p->left) {
            nodeQueue.push(p->left);
            ++isLeaf;
        }
        if (p->right) {
            nodeQueue.push(p->right);
            ++isLeaf;
        }

        if (!isLeaf)
            WPL += length * p->element;

        if (last == p) {
            ++length;
            last = nodeQueue.back();
        }

    }

    return WPL;
}

/**
 * 请设计一个算法，将给定的表达式树（二叉树）转换为等价的中缀表达式（通过括号反应操作符的计算次序）并输出。
 */


void convert(BinaryNode *&root, int deep) {
    if (root == nullptr)
        return;
    else if (root->left == nullptr && root->right == nullptr)
        std::cout << root->element;
    else {
        if (deep > 1)
            std::cout << "(";
        convert(root->left, deep + 1);
        std::cout << root->element;
        convert(root->right, deep + 1);
        if (deep > 1)
            std::cout << ")";
    }

}

void convertAstToInOrder(BinaryNode *&root) {
    convert(root, 1);
}


/**
 * 判断一棵树是否是二叉查找树
 */

bool assertBinarySearchTree(BinaryNode *&root) {
    bool leftS = true, rightS = true;
    if (root) {

        leftS = assertBinarySearchTree(root->left);
        rightS = assertBinarySearchTree(root->right);

        /**
        if((root->left && root->left->element > root->element) ||
            (root->right && root->right->element < root->element))
            return false;
            */

        if (root->left && root->left->element > root->element)
            leftS = false;
        if (root->right && root->right->element < root->element)
            rightS = false;

    }

    return leftS && rightS;
}

/**
 * 设计一个算法，求出指定节点在给定二叉排序树中的层次
 */

int getLevel(BinaryNode *&root, int x, int level) {
    if (root == nullptr)
        return -1;
    if (root->element == x)
        return level;
    else if (x < root->element)
        return getLevel(root->left, x, level + 1);
    else if (x > root->element)
        return getLevel(root->right, x, level + 1);

}

int getLevel(BinaryNode *&root, int x) {
    return getLevel(root, x, 1);
}


/**
 * 利用二叉树遍历的思想编写一个判断二叉树是否是平衡二叉树的算法
 */

//这个算法效率太低了
bool balance(BinaryNode *&root) {
    bool leftS = true, rightS = true;
    if (root) {
        if (std::abs((height(root->left) - height(root->right))) > 1)
            return false;

        leftS = balance(root->left);
        rightS = balance(root->right);

    }

    return leftS && rightS;

}

//给出一个更好的算法，忽略驱动程序
void balance(BinaryNode *&root, int &b, int &h) {
    //标记左子树的平衡情况和高度
    int bl = 0, hl = 0;
    //标记右子树的平衡情况和高度
    int br = 0, hr = 0;
    if (root == nullptr) {
        b = 1;
        h = 0;
    } else if (root->left == nullptr && root->right == nullptr) {
        b = 1;
        h = 1;
    } else {
        balance(root->left, bl, hl);
        balance(root->right, br, hr);

        h = std::max(hl, hr) + 1;

        if(std::abs(hl - hr) > 1 )
            b = 0;
        else
            b = bl && br;
    }
}

/**
 * 设计一个算法，求出给定二叉排序树中最小和最大的关键字
 */

bool findMin(BinaryNode * & root, int & x)
{
    if(root == nullptr)
        return false;
    if(root->left == nullptr) {
        x = root->element;
        return true;
    }
    else {
        return findMin(root->left, x);
    }
}

bool findMax(BinaryNode * & root, int & x)
{
    if(root == nullptr)
        return false;
    if(root->right == nullptr) {
        x = root->element;
        return true;
    }
    else {
        return findMax(root->left, x);
    }
}

/**
 * 设计一个算法，从大到小输出二叉排序树中所有值不小于k的关键字
 */

//要有序我觉得中序遍历是最合适的，可以利用类似于查找的方法
//首先利用查找，找到值为k的节点之后，返回k的右儿子节点，再对其进行中序遍历，并依次入栈，最后只需要弹出栈中内容，即是从大到小的顺序


//查找程序
BinaryNode * findK(BinaryNode * & root, int k)
{
    if(root == nullptr)
        return nullptr;
    if(root->element == k)
        return root;
    else if(k < root->element)
        return findK(root->left, k);
    else
        return findK(root->right, k);
}

//假设root是K节点的右儿子节点，进行中序遍历
void greaterThanK(BinaryNode * & root, stack<int> & numStack)
{
    if(root)
    {
        greaterThanK(root->left, numStack);
        numStack.push(root->element);
        greaterThanK(root->right, numStack);
    }
}

//驱动程序
void printGreaterThanK(BinaryNode * & root, int k)
{
    BinaryNode * kNode = findK(root, k);
    if(kNode == nullptr)
        return;

    stack<int> numStack;

    greaterThanK(kNode->right, numStack);

    while (!numStack.empty())
    {
        std::cout<<numStack.top()<<" ";
        numStack.pop();
    }
}

/**
 * 计算二叉树的节点的总个数
 */
int getNumberOfNodes(BinaryNode * & root)
{
    if(root) {
        return getNumberOfNodes(root->left) + getNumberOfNodes(root->right) + 1;
    }
    else {
        return 0;
    }
}

/**
 * 设给定权值w = {5, 7, 2, 3, 6, 8, 9}，试构造关于w的一颗哈夫曼树，并求其加权路径长度WPL
 */





#endif //DATA_STRUCTURE_CPP_BINARY_TREE_ALGORITHM_H
