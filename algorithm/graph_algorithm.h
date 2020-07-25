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

using std::string;
using std::vector;
using std::queue;

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

    Vertex(int i, string name): id(i), name(std::move(name)) {}

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








#endif //DATA_STRUCTURE_CPP_GRAPH_ALGORITHM_H
