//
// Created by 胡凌瑞 on 2020/7/7.
//

#ifndef DATA_STRUCTURE_CPP_GRAPH_H
#define DATA_STRUCTURE_CPP_GRAPH_H

#include <utility>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <list>
#include "dsexceptions.h"
#include <iostream>
#include <set>
#include "DisjSets.h"


using std::priority_queue;
using std::string;
using std::vector;
using std::map;
using std::queue;
using std::set;
using std::list;

/**
 * 图论算法的一些实现,包括
 * 图的邻接表实现
 * Dijkstra
 * Prim
 * Kruskal
 * DFS
 * Deep First Spanning Tree
 * Find Articulation point
 * Eular Tour
 * Detemine the Edge type in directed graph
 * find Strongly Connected Component
 * @tparam Object
 */


template<typename Object>
class Graph {

public:

    Graph() : currentSize(0) {}

    void addVertex(string key, const Object &x, int id) {
        vertexMap[key] = new Vertex(x, id, key);
        ++currentSize;
    }

    void addVertex(string key, Object &&x, int id) {
        vertexMap[key] = new Vertex(std::move(x), id, key);
        ++currentSize;
    }

    void addEdge(const string &from, const string &to, int weight = 1) {

        Vertex *fromNode = vertexMap[from];

        //更新from节点的出度
        ++fromNode->outDegree;
        //更新to节点的入度
        ++vertexMap[to]->inDegree;

        fromNode->adjacencyList.push_back(new Edge(from, to, weight));

    }

    void addTwoSideEdge(const string &v1, const string &v2, int weight = 1) {
        addEdge(v1, v2, weight);
        addEdge(v2, v1, weight);
    }


    void removeEdge(const string &from, const string &to) {
        //这个函数里还应该更新入度和出度
        Vertex *fromNode = vertexMap[from];
        list<Edge*> &refList = fromNode->adjacencyList;
        auto itr = fromNode->adjacencyList.begin();
        for (; itr != fromNode->adjacencyList.end() && (*itr)->to != to; ++itr);
        refList.erase(itr);
    }

    void removeAllEdges() {
        for (auto &pair: vertexMap)
            pair.second->adjacencyList.clear();
    }


    /**
     * 图：有向图
     * 计算无权值图的路径
     * @param key
     */
    void unweightedPath(string key) {
        unweighted(vertexMap[key]);
    }

    /**
     * 图：有向图
     * Dijkstra算法求最短路径
     * @param key
     */
    void dijkstra(string key) {
        dijkstra(vertexMap[key]);
    }

    /**
     * 图：有向图
     * 拓扑排序
     */
    void topSort() {
        topsort();
    }

    /**
     * 图：有向图
     * 最大流算法
     */
    void maxFlow() {
        Graph<Object> flowGraph = *this;
        Graph<Object> residualGraph = *this;

    }

    /**
     * 图：有向图
     * 计算带有负值边的图的路径
     * @param key
     */
    void weightedNegative(string key) {

        set<string> isInQueue;
        Vertex *thisVertex = vertexMap[key];
        queue<Vertex *> q;

        for (auto &pair: vertexMap) {
            pair.second->dist = INFINITY;
        }

        thisVertex->dist = 0;
        q.push(thisVertex);
        isInQueue.insert(thisVertex->name);

        while (!q.empty()) {
            Vertex *v = q.front();
            q.pop();

            for (auto &edge: v->adjacencyList) {
                Vertex *w = vertexMap[edge->to];
                int cvw = edge->weight;

                if (v->dist + cvw < w->dist) {
                    w->dist = v->dist + cvw;
                    w->path = v->name;
                    //如果w不在队列里面，则把w加入队列
                    if (!isInQueue.count(w->name))
                        q.push(w);
                }
            }
        }

    }

    /**
     * 图：无向图
     * prim算法生成最小生成树
     * @param primTree
     */
    void prim(Graph<Object> &primTree) {

        for (auto &pair: vertexMap)
            primTree.addVertex(pair.first, pair.second->element, pair.second->id);

        int totalUnknown = 0;
        for (auto &pair: vertexMap) {
            pair.second->known = false;
            pair.second->dist = INFINITY;
            ++totalUnknown;
        }

        // 这里选择最小图中的第一个vertex作为起点
        Vertex *s = vertexMap.begin()->second;
        s->dist = 0;
        s->path = "";

        while (totalUnknown > 0) {
            //找unknown中有最小dist的节点
            int minDist = INFINITY;
            Vertex *v;
            for (auto &pair: vertexMap) {
                if (pair.second->known == false && pair.second->dist < minDist) {
                    v = pair.second;
                    minDist = v->dist;
                }
            }

            //把节点设为已知
            v->known = true;
            --totalUnknown;

            //对于每一个和v邻接w，比较dist
            for (auto &edge: v->adjacencyList) {
                Vertex *w = vertexMap[edge->to];
                if (!w->known) {
                    int cvw = edge->weight;

                    if (cvw < w->dist) {
                        w->dist = cvw;
                        w->path = v->name;
                    }
                }
            }
        }

        //添加边到生成树中
        for (auto &pair: vertexMap) {
            Vertex *&v = pair.second;
            if (v->path == "")
                continue;
            primTree.addTwoSideEdge(v->name, v->path, v->dist);
        }

    }

    /**
     * 图：无向图
     * kruskal算法生成最小生成树
     * @param mst
     */
    void kruskal(Graph<Object> &mst) {
        //边的集合
        vector<Edge> edges;

        //把图中的节点加入到mst中
        for (auto &pair: vertexMap) {
            mst.addVertex(pair.first, pair.second->element, pair.second->id);
            //把边加到edges当中
            for (auto &edge: pair.second->adjacencyList)
                edges.push_back(*edge);
        }

        DisjSets disjSets(mst.size());

        //我们需要一个最小堆，这里是它的比较器
        auto cmp = [](const Edge &e1, const Edge &e2) -> bool { return e1.weight > e2.weight; };
        priority_queue<Edge, vector<Edge>, decltype(cmp)> priorityQueue(cmp, edges);

        vector<Edge> mstVec;

        while (mstVec.size() != mst.size() - 1) {
            //获取最小的边
            Edge e = priorityQueue.top();
            priorityQueue.pop();

            int uset = disjSets.find(vertexMap[e.from]->id);
            int vset = disjSets.find(vertexMap[e.to]->id);

            //两个属于两个集合，可以添加边
            if (uset != vset) {
                mstVec.push_back(e);
                disjSets.unionSets(uset, vset);
            }
        }

        //给树加边
        for (auto &edge : mstVec)
            mst.addTwoSideEdge(edge.from, edge.to, edge.weight);

    }

    /**
     * 图：无向图
     * 深度优先搜索
     * @param key
     */
    void dfs(string key) {
        Vertex *s = vertexMap[key];
        dfs(s);
    }

    /**
     * 图：无向图
     * 生成 深度优先生成树
     * @param key
     * @param mst
     */
    void dfSpanningTree(string key, Graph<Object> &mst) {
        //把图中的节点加入到mst中
        for (auto &pair: vertexMap)
            mst.addVertex(pair.first, pair.second->element, pair.second->id);

        Vertex *s = vertexMap[key];
        dfSpanningTree(s, mst);

    }

    /**
     * 图：无向图
     * 求出非双连通图中的割点
     * @param key
     */
    void findArt(string key) {
        Vertex *s = vertexMap[key];
        findArt(s);


        if (sonOfRoot > 1)
            std::cout << s->name << " is an articulation point" << std::endl;

    }

    /**
     * 图：无向图
     * 欧拉回路的算法
     * @param key
     * @return
     */
    list<string> eulerTour(string key) {

        int flag = 0;
        list<string> trace;
        list<string> tmpArray;
        Vertex *v = vertexMap[key];
        Edge *pEdge;

        //第一轮回，先找到一个环路
        dfsForEulerTour(v, v->name, pEdge, tmpArray, flag);
        trace = tmpArray;

        //找到一个没有被访问的边，然后从它的一个节点开始访问
        //这里可以优化，因为如果每次都对所有邻接点扫描，都会达到O(V^2)的复杂度
        Edge *unvisited = findUnvisitedEdge();

        //如果存在还没有被访问的边，开始继续搜索
        for (tmpArray.clear(), flag = 0;
             unvisited != nullptr; unvisited = findUnvisitedEdge(), tmpArray.clear(), flag = 0) {
            dfsForEulerTour(vertexMap[unvisited->from], unvisited->from, pEdge, tmpArray, flag);
            auto tmpItr = tmpArray.begin();
            auto traceItr = trace.begin();
            //把trace的迭代器移到tmp开头元素相等的位置
            for (auto &s: trace) {
                if (s != *tmpItr)
                    ++traceItr;
            }

            trace.erase(traceItr++);

            for (auto &s: tmpArray)
                trace.insert(traceItr, s);

        }

        return trace;

    }

    /**
     * 图：有向图
     * 标记图中各个边的类型，有 树边，前向边，背向边，交叉边
     * 通过这个可以得到一个有向图是否是无圈图，一个有向图是无圈图，当且仅当它没有背向边
     */
    void dfsToDetermineTypeInDirected() {

        //遍历所有节点，因为是有向图，所以一次深度搜索可能遍历不完全
        for(auto & pair: vertexMap) {
            Vertex * v = pair.second;
            if(!v->visited)
                dfsForDirected(v);
        }

    }

    /**
     * 图：有向图
     * 寻找强连通分支
     */
    void findStronglyConnectedComponents()
    {
        //遍历所有节点，因为是有向图，所以一次深度搜索可能遍历不完全，第一遍是为了给节点编号，用于下一次遍历
        for(auto & pair: vertexMap) {
            Vertex * v = pair.second;
            if(!v->visited)
                dfsForDirected(v);
        }

        reverseEdgeDirection();
        setAllNotVisited();

        //用一个数组来存储第一次遍历得到的序号，做一个简单的映射
        vector<string> keyStore(currentSize + 1);
        for(auto & pair: vertexMap) {
            Vertex * & v = pair.second;
            keyStore[v->num] = v->name;
        }

        //对节点进行从大到小的序号遍历
        for(auto itr = keyStore.end() - 1; itr >= keyStore.begin(); --itr) {
            //找到一个没有被访问的节点
            if(*itr != NULL_STRING) {
                std::cout<< "{ ";
                dfsForDirectedReverse(vertexMap[*itr], keyStore);
                std::cout<<"}"<<std::endl;
            }
        }

    }


    /**
     * 图：有向图
     * 用于打印从一个节点到另一个节点的路径，需要先调用Dijkstra
     * @param from
     * @param to
     */
    void printPath(string from, string to) {
        vector<string> path;
        Vertex *toNode = vertexMap[to];

        path.push_back(to);

        while (toNode->name != from) {
            path.push_back(toNode->path);
            toNode = vertexMap[toNode->path];
        }

        for (auto itr = --path.end(); itr >= path.begin(); --itr)
            std::cout << *itr << " ";

    }

    int size() {
        return currentSize;
    }

private:

    struct Edge;
    struct Vertex;

    enum EdgeType {
        Back, Forward, Cross, Normal
    };

    static const string NO_FATHER;
    static const string NULL_STRING;

    struct Edge {
        string from;
        string to;
        int weight;

        EdgeType type = Normal;

        //for Euler Tour
        bool visited = false;

        explicit Edge(string from, string t, int w = 0) : from(std::move(from)), to(std::move(t)), weight(w) {}

    };

    struct Vertex {
        Object element;

        //用于标示一个节点，用于不相交集
        int id;

        string name;

        list<Edge *> adjacencyList;

        int inDegree;
        int outDegree;

        //用于拓扑排序
        int topNum;

        //用于计算路径长度
        int dist = 0;
        //用于标明路径
        string path;

        //用于 Dijkstra
        bool known = false;

        //用于DFS
        bool visited = false;

        //用于割点的计算
        int num = 0;
        int low = 0;
        string parent = NO_FATHER;

        explicit Vertex(const Object &ele, int i, string n) : element(ele), id(i), adjacencyList(0), inDegree(0),
                                                              outDegree(0), topNum(0), name(std::move(n)) {}

        explicit Vertex(Object &&ele, int i, string n) : element(std::move(ele)), id(i), adjacencyList(0), inDegree(0),
                                                         outDegree(0), topNum(0), name(std::move(n)) {}

    };

    int currentSize;
    //counter用于计算割点
    int counter = 1;
    //计算深度生成树的根节点的儿子节点数
    int sonOfRoot = 0;
    map<string, Vertex *> vertexMap;
    map<string, int> inDegreeOfEachVertex;

    void updateInDegreeTable() {
        for (auto itr = vertexMap.begin(); itr != vertexMap.end(); ++itr)
            inDegreeOfEachVertex[itr->first] = itr->second->inDegree;
    }

    void dfsForDirected(Vertex *v) {

        v->visited = true;

        //对于所有邻接v的节点w,执行如下操作
        for (auto &edge : v->adjacencyList) {
            Vertex *w = vertexMap[edge->to];

            //如果节点已经被访问过了，这里面就要确定边的三个类型之一
            if(w->visited) {
                //这里如果w是v的祖先节点，我们可以判定w和v在同一棵树中，且v指向w的是背向边
                if (isFather(v, w))
                    edge->type = Back;
                else if (isFather(w,v))
                    edge->type = Forward;
                else
                    edge->type = Cross;

            } else if (!w->visited) {
                //没有被访问过，就需要继续进行深度遍历，并且把两个点加入到集合中
                w->parent = v->name;

                dfsForDirected(w);

            }
        }

        v->num = counter++;

    }

    void dfsForDirectedReverse(Vertex *v, vector<string> & keyStore) {

        v->visited = true;
        keyStore[v->num] = NULL_STRING;

        std::cout<<v->name<<" ";

        //对于所有邻接v的节点w,执行如下操作
        for (auto &edge : v->adjacencyList) {
            Vertex *w = vertexMap[edge->to];

            //如果节点已经被访问过了，这里面就要确定边的三个类型之一
            if(w->visited) {
                //这里如果w是v的祖先节点，我们可以判定w和v在同一棵树中，且v指向w的是背向边
                if (isFather(v, w))
                    edge->type = Back;
                else if (isFather(w,v))
                    edge->type = Forward;
                else
                    edge->type = Cross;

            } else if (!w->visited) {
                //没有被访问过，就需要继续进行深度遍历，并且把两个点加入到集合中
                w->parent = v->name;

                dfsForDirectedReverse(w, keyStore);

            }
        }

    }

    void reverseEdgeDirection()
    {
        vector<Edge *> edges;
        for(auto & pair: vertexMap) {
            Vertex *v = pair.second;
            for(auto & edge : v->adjacencyList)
                edges.push_back(edge);
        }

        removeAllEdges();

        for(auto & edge: edges) {
            addEdge(edge->to, edge->from);
        }

    }


    //用于确定有向图的边类型的工具函数,找到一个节点的父亲节点
    //判断a是否是b的后继节点
    bool isFather(Vertex * a, Vertex * b)
    {
        if(a->parent == NO_FATHER)
            return false;
        else if(a->parent == b->name)
            return true;
        else
            return isFather(vertexMap[a->parent], b);

    }

    void topsort() {

        updateInDegreeTable();

        queue<Vertex *> q;
        int counter = 0;

        for (auto &itr : inDegreeOfEachVertex) {
            if (itr.second == 0)
                q.push(vertexMap[itr.first]);
        }

        while (!q.empty()) {
            Vertex *v = q.front();
            q.pop();

            v->topNum = ++counter;
            std::cout << v->element << " ";

            //遍历出的每一个都是Edge的指针
            for (auto &edge: v->adjacencyList) {
                if (edge != nullptr && --inDegreeOfEachVertex[edge->to] == 0)
                    q.push(vertexMap[edge->to]);
            }

        }

        if (counter != currentSize)
            throw CycleFoundException();


    }


    static const int INFINITY = INT_MAX;

    void unweighted(Vertex *s) {
        queue<Vertex *> q;

        for (auto &pair: vertexMap)
            pair.second->dist = INFINITY;

        s->dist = 0;
        q.push(s);

        while (!q.empty()) {
            //取出队列中的节点
            Vertex *v = q.front();
            q.pop();

            for (auto &edge: v->adjacencyList)
                if (edge != nullptr && vertexMap[edge->to]->dist == INFINITY) {
                    vertexMap[edge->to]->dist = v->dist + 1;
                    vertexMap[edge->to]->path = v->name;
                    q.push(vertexMap[edge->to]);
                }

        }

    }

    void dijkstra(Vertex *s) {
        //初始化所有的节点为unknown
        int totalUnknown = 0;
        for (auto &pair: vertexMap) {
            pair.second->known = false;
            pair.second->dist = INFINITY;
            ++totalUnknown;
        }

        s->dist = 0;

        while (totalUnknown > 0) {
            Vertex *minVertex;
            int minDist = INFINITY;
            //首先，需要寻找拥有最短路径的Unknown，然后标记为known
            for (auto &pair: vertexMap) {
                if (pair.second->known == false && pair.second->dist < minDist) {
                    minVertex = pair.second;
                    minDist = minVertex->dist;
                }
            }

            //标记为已知
            minVertex->known = true;
            --totalUnknown;

            //遍历邻接节点
            for (auto &edge: minVertex->adjacencyList) {
                Vertex *w = vertexMap[edge->to];
                if (!w->known) {
                    //从minVertex到w的距离
                    int cvw = edge->weight;

                    //如果从这个点到w距离小于之前的距离，则更新w的距离,并且更新路径
                    if (minVertex->dist + cvw < w->dist) {
                        w->dist = minVertex->dist + cvw;
                        w->path = minVertex->name;
                    }
                }
            }
        }
    }

    void dfs(Vertex *v) {
        v->visited = true;
        //遍历邻接节点
        for (auto &edge: v->adjacencyList) {
            Vertex *w = vertexMap[edge->to];
            if (!w->visited)
                dfs(w);
        }
    }

    void dfsForEulerTour(Vertex *v, string startName, Edge *&pEdge, list<string> &tmpArray, int &flag) {
        if (flag == 1) {
            tmpArray.push_back(startName);
            return;
        }

        tmpArray.push_back(v->name);

        for (auto &edge: v->adjacencyList) {
            Vertex *w = vertexMap[edge->to];
            if (!edge->visited) {
                markVisitedOfTwoSide(v->name, w->name);
                pEdge = edge;
                if (w->name == startName)
                    flag = 1;
                dfsForEulerTour(w, startName, pEdge, tmpArray, flag);

            }
        }

    }


    void dfSpanningTree(Vertex *s, Graph<Object> &mst) {
        s->visited = true;
        for (auto &edge: s->adjacencyList) {
            Vertex *w = vertexMap[edge->to];
            if (!w->visited) {
                mst.addTwoSideEdge(edge->from, edge->to, edge->weight);
                dfSpanningTree(w, mst);
            }

        }
    }

    void findArt(Vertex *s) {
        s->visited = true;
        s->low = s->num = counter++;
        for (auto &edge: s->adjacencyList) {
            Vertex *w = vertexMap[edge->to];
            if (!w->visited) //前向边
            {
                w->parent = s->name;
                if (vertexMap[w->parent]->num == 1)
                    ++sonOfRoot;
                findArt(w);
                if (s->num != 1 && w->low >= s->num)
                    std::cout << s->name << " is an articulation point" << std::endl;
                s->low = std::min(s->low, w->low);
            } else if (s->parent != w->name) //背向边
            {
                s->low = std::min(w->num, s->low);
            }
        }

    }


    void markVisitedOfTwoSide(const string &v1, const string &v2) {
        Vertex *&v1Node = vertexMap[v1];
        Vertex *&v2Node = vertexMap[v2];

        for (auto &edge: v1Node->adjacencyList) {
            if (edge->to == v2)
                edge->visited = true;
        }

        for (auto &edge: v2Node->adjacencyList) {
            if (edge->to == v1)
                edge->visited = true;
        }

    }

    void setAllNotVisited() {
        for (auto &pair: vertexMap)
            if (pair.second->visited == true)
                pair.second->visited = false;
    }

    Edge *findUnvisitedEdge() {
        for (auto &pair: vertexMap) {
            Vertex *v = pair.second;
            for (auto &edge: v->adjacencyList)
                if (!edge->visited)
                    return edge;
        }
        return nullptr;
    }

};

template <typename Object>
const string Graph<Object>::NO_FATHER = "NO_FATHER";

template <typename Object>
const string Graph<Object>::NULL_STRING= "NO_STRING";

#endif //DATA_STRUCTURE_CPP_GRAPH_H
