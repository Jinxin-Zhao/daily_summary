#ifndef  FLOYD_DIJKSTRA_H_
#define  FLOYD_DIJKSTRA_H_

/*
	Name: Floyd & Dijkstra算法
input example：
5 7
0 1 100
0 2 30
0 4 10
2 1 60
2 3 60
4 3 50
3 1 10
output:
0 4 3 1 70
0 2 30
0 4 3 60
0 4 10
*/
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <stack>
#include <vector>
#include <list>

//图的邻接矩阵表示法
#define MaxVertexNum 100 //最大顶点数设为100
#define INFINITY 65535 //无穷设为双字节无符号整数的最大值65535
typedef int Vertex;//用顶点下标表示顶点，为整型cd
typedef int WeightType;//边的权值设为整型
typedef char DataType;//顶点存储的数据类型设为字符型
//图结点的定义
typedef struct GNode *PtrToGNode;
struct GNode {
    int Nv;//顶点数
    int Ne;//边数
    WeightType G[MaxVertexNum][MaxVertexNum];//邻接矩阵，表示顶点间的相邻关系
    DataType Data[MaxVertexNum];//存顶点的数据
    //注意：如果顶点无数据，此时Data[]可以不用出现
};
typedef PtrToGNode MGraph;//用邻接矩阵存储图的类型
//边的定义
typedef struct ENode *PtrToENode;
struct ENode {
    int V1, V2;//边的两个顶点
    WeightType Weight;//边的权重
};
typedef PtrToENode Edge;

//////////////
class DirectedWeightAlgorithm {
public:
    DirectedWeightAlgorithm() {
        m_VertexNum = 0;
        m_EdgeNum = 0;
        init();
    }

public:
    int Floyd(MGraph Graph,int **dist,int **path,Vertex S);
    void InitArray(int **p,int vertexNum);
    void PrintPath(int **path,Vertex S,Vertex W);
    //
    bool Dijkstra(MGraph Graph, int *dist, int *path, Vertex S);
    MGraph getGraph(){return m_Graph;}
    int * getDist(){return m_dist;}
    int * getPath(){return m_path;}
    int getVertexNum(){return m_VertexNum;}
    void PrintPath(int *path, Vertex W, Vertex S);
    //
    // function to add an edge to graph
    void addEdge(int v, int w);
    // prints a Topological Sort of the complete graph
    void topologicalSort();
    //
    void topologicalSort_2();
private:
    void init();
    MGraph CreateGraph(int VertexNum);
    void InsertEdge(MGraph Graph, Edge E);
    Vertex FindMinDist(MGraph Graph, int *dist, int *collected);

private:
    int * m_dist;
    int * m_path;
    MGraph  m_Graph;
    //No. of vertices
    int m_VertexNum;
    int m_EdgeNum;

    //for topologic sort
    bool * m_visit;
    // Pointer to an array containing adjacency listsList
    std::list<int> * m_adj;
    std::stack<int> m_stack;
    // A function used by topologicalSort
    void topologicalSortHelper(int v, bool visited[], std::stack<int>& Stack);
    //
    void topologicDegree(int v,bool visit[],std::stack<int>& Stack);
};


#endif
