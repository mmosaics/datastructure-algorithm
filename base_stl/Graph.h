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
 * @tparam Object
 */
template <typename Object>
class Graph
{

public:

    Graph(): currentSize(0){}

    void addVertex(string key, const Object & x)
    {
        vertexMap[key] = new Vertex(x, key);
        ++currentSize;
    }

    void addVertex(string key, Object && x)
    {
        vertexMap[key] = new Vertex(std::move(x), key);
        ++currentSize;
    }

    void addEdge(const string & from, const string & to, int weight)
    {

        Vertex * fromNode = vertexMap[from];

        //更新from节点的出度
        ++fromNode->outDegree;
        //更新to节点的入度
        ++vertexMap[to]->inDegree;

        fromNode->adjacencyList.push_back(new Edge(from, to, weight));

    }

    void addTwoSideEdge(const string & v1, const string & v2, int weight)
    {
        addEdge(v1, v2, weight);
        addEdge(v2, v1, weight);
    }

    void removeEdge(const string & from, const string & to)
    {
        Vertex * fromNode = vertexMap[from];
        list<Edge> & refList = fromNode->adjacencyList;
        auto itr = fromNode->adjacencyList.begin();
        for(; itr != fromNode->adjacencyList.end() && *itr->to != to; ++itr);
        refList.erase(itr);
    }

    void removeAllEdges()
    {
        for(auto & pair: vertexMap)
            pair.second->adjacencyList.clear();
    }

    void unweightedPath(string key)
    {
        unweighted(vertexMap[key]);
    }

    void dijkstra(string key)
    {
        dijkstra(vertexMap[key]);
    }

    void topSort()
    {
        topsort();
    }

    void maxFlow()
    {
        Graph<Object> flowGraph = *this;
        Graph<Object> residualGraph = *this;

    }

    void weightedNegative(string key)
    {

        set<string> isInQueue;
        Vertex * thisVertex = vertexMap[key];
        queue<Vertex *> q;

        for(auto & pair: vertexMap)
        {
            pair.second->dist = INFINITY;
        }

        thisVertex->dist = 0;
        q.push(thisVertex);
        isInQueue.insert(thisVertex->name);

        while(!q.empty())
        {
            Vertex * v = q.front();
            q.pop();

            for(auto & edge: v->adjacencyList)
            {
                Vertex * w = vertexMap[edge->to];
                int cvw = edge->weight;

                if(v->dist + cvw < w->dist)
                {
                    w->dist = v->dist + cvw;
                    w->path = v->name;
                    //如果w不在队列里面，则把w加入队列
                    if(!isInQueue.count(w->name))
                        q.push(w);
                }
            }
        }

    }

    void prim(Graph<Object> & primTree)
    {

        for(auto & pair: vertexMap)
            primTree.addVertex(pair.first, pair.second->element);

        int totalUnknown = 0;
        for(auto & pair: vertexMap)
        {
            pair.second->known = false;
            pair.second->dist = INFINITY;
            ++totalUnknown;
        }

        // 这里选择最小图中的第一个vertex作为起点
        Vertex * s = vertexMap.begin()->second;
        s->dist = 0;
        s->path = "";

        while (totalUnknown > 0)
        {
            //找unknown中有最小dist的节点
            int minDist = INFINITY;
            Vertex * v;
            for(auto & pair: vertexMap)
            {
                if(pair.second->known == false && pair.second->dist < minDist)
                {
                    v = pair.second;
                    minDist = v->dist;
                }
            }

            //把节点设为已知
            v->known = true;
            --totalUnknown;

            //对于每一个和v邻接w，比较dist
            for(auto & edge: v->adjacencyList)
            {
                Vertex * w = vertexMap[edge->to];
                if(!w->known) {
                    int cvw = edge->weight;

                    if (cvw < w->dist) {
                        w->dist = cvw;
                        w->path = v->name;
                    }
                }
            }
        }

        //添加边到生成树中
        for(auto & pair: vertexMap)
        {
            Vertex * & v = pair.second;
            if(v->path == "")
                continue;
            primTree.addTwoSideEdge(v->name, v->path, v->dist);
        }

    }

    void kruskal(Graph<Object> & mst)
    {
        //边的集合
        vector<Edge> edges;

        //把图中的节点加入到mst中
        for(auto & pair: vertexMap) {
            mst.addVertex(pair.first, pair.second->element);
            //把边加到edges当中
            for(auto & edge: pair.second->adjacencyList)
                edges.push_back(*edge);
        }

        DisjSets disjSets(mst.size());

        //我们需要一个最小堆，这里是它的比较器
        auto cmp = [](const Edge & e1, const Edge & e2)->bool{return e1.weight > e2.weight;};
        priority_queue<Edge, vector<Edge>, decltype(cmp)> priorityQueue(cmp, edges);

        vector<Edge> mstVec;

        while (mstVec.size() != mst.size() - 1)
        {
            //获取最小的边
            Edge e = priorityQueue.top();
            priorityQueue.pop();

            int uset = disjSets.find(vertexMap[e.from]->element);
            int vset = disjSets.find(vertexMap[e.to]->element);

            //两个属于两个集合，可以添加边
            if( uset != vset)
            {
                mstVec.push_back(e);
                disjSets.unionSets(uset, vset);
            }
        }

        //给树加边
        for(auto & edge : mstVec)
            mst.addTwoSideEdge(edge.from, edge.to, edge.weight);

    }

    void printPath(string from, string to)
    {
        vector<string> path;
        Vertex * toNode = vertexMap[to];

        path.push_back(to);

        while (toNode->name != from)
        {
            path.push_back(toNode->path);
            toNode = vertexMap[toNode->path];
        }

        for(auto itr = --path.end(); itr >= path.begin(); --itr )
            std::cout<<*itr<<" ";

    }

    int size()
    {
        return currentSize;
    }

private:

    struct Edge;
    struct Vertex;

    struct Edge
    {
        string from;
        string to;
        int weight;

        explicit Edge(string from, string t, int w = 0): from(std::move(from)),to(std::move(t)), weight(w) {}

    };

    struct Vertex
    {
        Object element;

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

        explicit Vertex(const Object & ele, string n): element(ele), adjacencyList(0), inDegree(0), outDegree(0), topNum(0), name(std::move(n)){}

        explicit Vertex(Object && ele, string n): element(std::move(ele)), adjacencyList(0), inDegree(0), outDegree(0), topNum(0), name(std::move(n)){}

    };

    int currentSize;
    map<string, Vertex *>  vertexMap;
    map<string, int> inDegreeOfEachVertex;

    void updateInDegreeTable()
    {
        for( auto itr = vertexMap.begin(); itr != vertexMap.end(); ++itr)
            inDegreeOfEachVertex[itr->first] = itr->second->inDegree;
    }


    void topsort() {

        updateInDegreeTable();

        queue<Vertex *> q;
        int counter = 0;

        for (auto &itr : inDegreeOfEachVertex) {
            if (itr.second == 0)
                q.push(vertexMap[itr.first]);
        }

        while (!q.empty())
        {
            Vertex * v = q.front();
            q.pop();

            v->topNum = ++counter;
            std::cout<< v->element<< " ";

            //遍历出的每一个都是Edge的指针
            for(auto & edge: v->adjacencyList) {
                if(edge != nullptr && --inDegreeOfEachVertex[edge->to] == 0)
                    q.push(vertexMap[edge->to]);
            }

        }

        if(counter != currentSize)
            throw CycleFoundException();


    }


    static const int INFINITY = INT_MAX;

    void unweighted(Vertex * s)
    {
        queue<Vertex *> q;

        for(auto & pair: vertexMap)
            pair.second->dist = INFINITY;

        s->dist = 0;
        q.push(s);

        while (!q.empty())
        {
            //取出队列中的节点
            Vertex * v = q.front();
            q.pop();

            for(auto & edge: v->adjacencyList)
                if(edge != nullptr && vertexMap[edge->to]->dist == INFINITY)
                {
                    vertexMap[edge->to]->dist = v->dist + 1;
                    vertexMap[edge->to]->path = v->name;
                    q.push(vertexMap[edge->to]);
                }

        }

    }

    void dijkstra(Vertex * s)
    {
        //初始化所有的节点为unknown
        int totalUnknown = 0;
        for(auto & pair: vertexMap)
        {
            pair.second->known = false;
            pair.second->dist = INFINITY;
            ++totalUnknown;
        }

        s->dist = 0;

        while (totalUnknown > 0)
        {
            Vertex * minVertex;
            int minDist = INFINITY;
            //首先，需要寻找拥有最短路径的Unknown，然后标记为known
            for(auto & pair: vertexMap)
            {
                if(pair.second->known == false && pair.second->dist < minDist) {
                    minVertex = pair.second;
                    minDist = minVertex->dist;
                }
            }

            //标记为已知
            minVertex->known = true;
            --totalUnknown;

            //遍历邻接节点
            for(auto & edge: minVertex->adjacencyList)
            {
                Vertex * w = vertexMap[edge->to];
                if(!w->known) {
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

};



#endif //DATA_STRUCTURE_CPP_GRAPH_H
