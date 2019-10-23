#ifndef  PRIME_KRUSKAL_H_
#define  PRIME_KRUSKAL_H_

#include <cstring>
#include <algorithm>

//weighted graph includes :
// 1)undirected weighted graph, shortest path is called minimum spanning tree(MST)[ Algorithm: --Prime(O(n^2))-- --Kruskal(O(eloge))-- ]
// 2)directed weighted graph,[ Algorithm:  --Floyd-- --Dijkstra(without negative edge)-- --Bellman_Ford-- ]

const int INF = 0XFFFFFFF;
const int N = 6;

inline int graph[][N]={
        {INF, 7,   4,   INF, INF, INF},
        {7,   INF, 6,   2,   INF, 4  },
        {4,   6,   INF, INF, 9,   8  },
        {INF, 2,   INF, INF, INF, 7  },
        {INF, INF, 9,   INF, INF, 1  },
        {INF, 4,   8,   7,   1,   INF}
};

typedef struct node{
    int val;
    int start;
    int end;
}UEdge[N * N / 2];

class UnDirectedWeightAlgorithm{
public:
    UnDirectedWeightAlgorithm(){
        memset(m_visited,false,sizeof(m_visited));
        memset(dist,0,sizeof(dist));
        memset(m_edge,0,sizeof(m_edge));
        memset(m_father,0,sizeof(m_father));
        memset(m_rank,0,sizeof(m_rank));
        init();
    }

public:
    int Prime(int cur);
    //
    int Kruskal(int cur);
    int getEdgeNum(){return num_edge;}
private:
    int init();
    //based on union-find algorithm
    void make_set();
    //1) find ancestor of current set by rank(depth)
    int find_set(int x);
    void union_set(int x,int y);
    //1) path compress
    int find_set_c(int x);
    void union_set_c(int x,int y);
private:
    bool m_visited[N];
    int  dist[N];
    int  m_father[N];
    int  m_rank[N];
    UEdge m_edge;
    int  num_edge;
};



#endif