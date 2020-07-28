//
// Created by 胡凌瑞 on 2020/7/22.
//

#ifndef DATA_STRUCTURE_CPP_GRAPH_ALGORITHM_H
#define DATA_STRUCTURE_CPP_GRAPH_ALGORITHM_H

#include <string>
#include <vector>
#include <iostream>
#include <exception>
#include "../base_structure/dsexceptions.h"
#include <queue>
#include "../base_structure/DisjSets.h"
#include <functional>

using std::string;
using std::vector;
using std::queue;
using std::priority_queue;

/**
 * 在数据结构模块，我已经完成了以邻接表为基础的图论算法，并且进行了封装
 * 这里就使用邻接矩阵的方法来完成图论算法
 * 本模块主要是为了练习图论算法，所以就直接使用图，因此算法就不以成员函数的方式实现，而是把图作为参数传递
 */

//先实现基本的结构

const int INFINITY = INT_MAX;

//图的节点
struct Vertex {
    int id;
    string name;

    //标识是否被访问过
    bool visited = false;

    //known域，用于dijkstra prim
    bool known = false;

    //标示路径，用于dijktra prim
    int path = -1;

    Vertex(int i, string name): id(i), name(std::move(name)) {}

};

//图的边
struct Edge {
    int from;
    int to;
    int weight;

    Edge(int f, int t, int w): from(f), to(t), weight(w){}

};

//默认都是public的，默认为无向图
class Graph {

public:
    explicit Graph(bool isDirectedGraph = false): directed(isDirectedGraph){}

    explicit Graph(bool isDirectedGraph = false, int numberOfVertex = 0): directed(isDirectedGraph), size(numberOfVertex)
    {
        for(int i = 0; i < numberOfVertex; ++i) {

            //初始化节点
            vertexList.push_back(new Vertex(i, string("v").append(std::to_string(i))));

            //初始化边
            vector<int> edgeOfVertex;
            edgeOfVertex.resize(numberOfVertex);
            for(auto & ele: edgeOfVertex)
                ele = INFINITY;

            edges.push_back(std::move(edgeOfVertex));
        }

    }

public:
    vector<Vertex *> vertexList;
    vector<vector<int>> edges;

    int size = 0;
    bool directed;

};


/**
 * 判断图G是否存在边<x，y>
 * @param G
 * @param x
 * @param y
 * @return
 */
bool adjacent(Graph & G, int x, int y) {
    return G.edges[x][y] != INFINITY;
}

/**
 * 向图中插入顶点
 * @param G
 * @param name
 * @return
 */
int insertVertex(Graph & G, const string & name)
{

    //用 size + 1 作为新节点的id
    int id = G.size++;
    G.vertexList.push_back(new Vertex(id, name));

    //更新邻接矩阵
    vector<int> newLine(G.size - 1, INFINITY);
    G.edges.push_back(std::move(newLine));

    //为其他边添加一列，代表加入了新节点
    for(auto & vec: G.edges)
        vec.push_back(INFINITY);

    return id;
}

/**
 * 删除id为x的节点，因为这里主要是学习算法，所以方便起见，执行懒惰删除，把边置为INFINITY即可
 * 考虑到实现方式，如果需要动态更新矩阵，用Map实现邻接矩阵是比较好的方法
 * @param G
 * @param x
 */
void removeVertex(Graph & G, int x)
{
    auto itr = G.vertexList.begin();
    for(; itr != G.vertexList.end(); ++itr)
    {
        if((*itr)->id == x)
            break;
    }
    int id = (*itr)->id;
    G.vertexList.erase(itr);

    for(auto & ele: G.edges[id])
        ele = INFINITY;

}


/**
 * 给节点添加一条边，若是无向图，则默认为1
 * @param G
 * @param x
 * @param y
 * @param weight
 * @return
 */
void addEdge(Graph & G, int x, int y, int weight = 1)
{
    //如果G是有向图
    if(G.directed) {
        G.edges[x][y] = weight;
    }
    //如果G是无向图，则添加双向边
    else {
        G.edges[x][y] = weight;
        G.edges[y][x] = weight;
    }
}


/**
 * 删除图的边
 * @param G
 * @param x
 * @param y
 */
void removeEdge(Graph & G, int x, int y)
{
    //如果G是有向图
    if(G.directed) {
        G.edges[x][y] = INFINITY;
    }
    //如果G是无向图，删除双向边
    else {
        G.edges[x][y] = INFINITY;
        G.edges[y][x] = INFINITY;
    }
}


/**
 * 返回id为x的节点第一个邻接点
 * @param G
 * @param x
 * @return
 */
int firstNeighbor(Graph & G, int x)
{
    vector<int> & edgeOfX = G.edges[x];

    for(int i = 0; i < edgeOfX.size(); ++i) {
        if(edgeOfX[i] != INFINITY)
            return i;
    }
    return -1;
}

/**
 * 设y是x的一个邻接点，返回除y节点外的下一个x的邻接点
 * @param G
 * @param x
 * @param y
 * @return
 */
int nextNeighbor(Graph & G, int x, int y) {
    vector<int> &edgeOfX = G.edges[x];

    for (int i = y+1; i < edgeOfX.size(); ++i) {
        if (i != y && edgeOfX[i] != INFINITY)
            return i;
    }
    return -1;
}

/**
 * 获取 x指向y的边的权值（有向图），或x到y的边的权值（无向图）
 * @param G
 * @param x
 * @param y
 * @return
 */
int getEdgeValue(Graph & G, int x, int y)
{
    if(x >= G.size || y >= G.size)
        throw ArrayIndexOutOfBoundsException();
    return G.edges[x][y];
}

/**
 * 基本同上
 * @param G
 * @param x
 * @param y
 * @param weight
 */
void setEdgeValue(Graph & G, int x, int y, int weight)
{
    if(x >= G.size || y >= G.size)
        throw ArrayIndexOutOfBoundsException();
    G.edges[x][y] = weight;
}

/**
 * 把图中所有节点置为未访问状态
 * @param G
 */
void setAllUnvisited(Graph & G) {

    for(int i = 0; i < G.size; ++i)
        G.vertexList[i]->visited = false;
}

/**
 * 把图中所有节点置为unknown状态
 * @param G
 */
void setAllUnknown(Graph & G) {

    for(int i = 0; i < G.size; ++i)
        G.vertexList[i]->known = false;
}

//图的广度优先遍历
/**
 * 和树的层序遍历一样，都需要用一个队列辅助，还要考虑图不是连通的，所以会有多个连通分量，所以都要遍历到
 */
void BFS(Graph & G, int v)
{
    //初始化一个队列
    queue<int> vertexQueue;
    //把第一个节点放入队列中
    vertexQueue.push(v);

    //如果队列非空
    while (!vertexQueue.empty())
    {
        //获取队首元素
        int v = vertexQueue.front();
        vertexQueue.pop();

        //标记为已访问
        G.vertexList[v]->visited = true;

        //遍历v的邻接节点，如果有，则入队
        for(int w = firstNeighbor(G, v); w >= 0; w = nextNeighbor(G, v, w))
        {
            //如果这个节点是v的邻接点，没有被访问过，则要入队
            if(!G.vertexList[w]->visited)
                vertexQueue.push(w);
        }

    }

}

//BFS算法求单源最短路径

/*
 * 和dijkstr大同小异，只是只能用于无权图
 */

void findMinDistanceBFS(Graph & G, int v, int* d)
{
    //首先把距离置为无穷大
    for(int i = 0; i < G.size; ++i)
        d[i] = INFINITY;
    //标记起始点距离为0
    d[v] = 0;
    queue<int> vertexQueue;
    vertexQueue.push(v);

    while (!vertexQueue.empty())
    {
        int tmp = vertexQueue.front();
        vertexQueue.pop();
        //出队，并标记为已访问
        G.vertexList[tmp]->visited = true;

        //遍历邻接节点
        for(int w = firstNeighbor(G, tmp); w >= 0; w = nextNeighbor(G, tmp, w))
        {
            if(!G.vertexList[w]->visited) {
                int newDis = d[tmp] + 1;
                if (newDis < d[w])
                    d[w] = newDis;
                vertexQueue.push(w);
            }
        }
    }
}


//图的深度优先遍历
void DFS(Graph & G, int v) {
    //visit(v);
    G.vertexList[v]->visited = true;
    for (int w = firstNeighbor(G, v); w >= 0; w = nextNeighbor(G, v, w)) {
        if(!G.vertexList[w]->visited)
            DFS(G, w);
    }
}
//如果图是非连通的，则需要对所有节点进行访问
void DFSTraverse(Graph & G) {
    setAllUnvisited(G);
    for(int v = 0; v < G.size; ++v)
        DFS(G, v);
}


//Prim算法
/**
 * prim算法是一种贪心，实质上也是层序遍历的一种应用
 * @param G
 * @param mst
 */
void primForMST(Graph & G, Graph & mst, int* d)
{
    //初始化距离数组
    for(int i = 0; i < G.size; ++i)
        d[i] = INFINITY;
    setAllUnknown(G);

    int totalUnknown = G.size;

    //首先置任意一个节点为known，并作为起始点构建生成树，这里选择0
    d[0] = 0;

    while (totalUnknown>0)
    {
        //首先选择目前拥有最短距离且为unknown的节点
        int minDist = INFINITY;
        int minVertex;
        for(int i = 0; i < G.size; ++i) {
            if(d[i] < minDist && !G.vertexList[i]->known) {
                minDist = d[i];
                minVertex = i;
            }
        }

        //经过循环后minVertex里存储的即是目前最短的节点，将它标记为known
        G.vertexList[minVertex]->known = true;
        --totalUnknown;

        //然后对minVertex的邻接节点进行遍历，更新距离表
        for(int w = firstNeighbor(G, minVertex); w >= 0; w = nextNeighbor(G, minVertex, w)) {
            if(!G.vertexList[w]->known ) {
                int weight = getEdgeValue(G, minVertex, w);
                if (weight < d[w]) {
                    d[w] = weight;
                    G.vertexList[w]->path = minVertex;
                }
            }
        }

    }

    //给mst加边
    for(int i = 0; i < G.size; ++i)
        if(G.vertexList[i]->path == -1)
            continue;
        else
            addEdge(mst, i, G.vertexList[i]->path, getEdgeValue(G, i, G.vertexList[i]->path));

}

void prim(Graph & G, Graph & mst)
{
    int * d = new int[G.size];
    primForMST(G, mst, d);
    delete[] d;
}


//Kruskal


void kruskal(Graph & G, Graph & mst)
{
    vector<Edge> edges;

    //我们需要先把边存储在一个堆中，邻接矩阵的边是以数组存储的，所以还需要进行一次遍历把所有边
    for(int i = 0; i < G.size; ++i) {
        for (int j = 0; j < G.size; ++j)
            if (adjacent(G, i, j))
                edges.emplace_back(i, j, getEdgeValue(G, i, j));
    }

    //先初始化一个堆，这应当是一个最小堆
    auto cmp = [](const Edge & e1, const Edge & e2)->bool {return e1.weight > e2.weight;};
    priority_queue<Edge, vector<Edge>, decltype(cmp)> priorityQueue(cmp, edges);

    //初始化一个不相交集
    DisjSets disjSets(G.size);

    while (!priorityQueue.empty())
    {
        //如果队列不为空，则会弹出最小的边
        Edge edge = priorityQueue.top();
        priorityQueue.pop();

        //如果这条边的两个节点属于不同的集合，则把他们加入到生成树中
        int fromSet = disjSets.find(edge.from);
        int toSet = disjSets.find(edge.to);

        //这两个节点属于不同集合
        if(fromSet != toSet) {
            addEdge(mst, edge.from, edge.to, edge.weight);
            disjSets.unionSets(fromSet, toSet);

        }
    }
}

//Dijkstra

void dijkstra(Graph & G, int u, int* d)
{
    //把所有置为unkown
    setAllUnknown(G);
    //把所有距离置为INFINITY
    for(int i = 0; i < G.size; ++i)
        d[i] = INFINITY;
    //因为u为起始点，所以把距离置为0
    d[u] = 0;

    int totalUnknown = G.size;
    while (totalUnknown > 0)
    {
        int minVertex = -1;
        int minDis = INFINITY;
        //找到距离最小的点，并标记为known
        for(int i = 0; i < G.size; ++i) {
            if(d[i] < minDis && !G.vertexList[i]->known) {
                minDis = d[i];
                minVertex = i;
            }
        }

        G.vertexList[minVertex]->known = true;
        --totalUnknown;

        //开始对minVertex的邻接节点更新，一定是对unkown节点进行操作
        for(int w = firstNeighbor(G, minVertex); w >= 0; w = nextNeighbor(G, minVertex, w)) {
            if(!G.vertexList[w]->known) {
                int newDis = d[minVertex] + getEdgeValue(G, minVertex, w);
                //如果新的距离小于当前距离，则更新
                if(newDis < d[w]) {
                    d[w] = newDis;
                    G.vertexList[w]->path = minVertex;
                }
            }
        }
    }
}

void dijkstra(Graph & G, int u)
{
    int * d = new int[G.size];
    dijkstra(G, u, d);
    delete[] d;
}

void printShortestPathByDijkstra(Graph & G, int source, int target)
{
    dijkstra(G, source);
    Vertex * v = G.vertexList[target];
    stack<int> tmpStack;
    while (v->id >= 0)
    {
        tmpStack.push(v->id);
        //如果v已经是起始节点，则退出循环
        if(v->path < 0)
            break;
        v = G.vertexList[v->path];
    }

    while(!tmpStack.empty())
    {
        if(tmpStack.size() > 1)
            std::cout<< tmpStack.top() << "->";
        else if(tmpStack.size() == 1)
            std::cout<< tmpStack.top();
        tmpStack.pop();
    }
    std::cout<<std::endl;
}

#endif //DATA_STRUCTURE_CPP_GRAPH_ALGORITHM_H
