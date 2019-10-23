#include "Prime-Kruskal.h"

int UnDirectedWeightAlgorithm::Prime(int cur) {
    int index;
    int sum = 0;
    m_visited[cur] = true;
    for(int i = 0; i < N; ++i){
        dist[i] = graph[cur][i];
    }

    for(int i = 1; i < N; ++i){
        int mincost = INF;
        for(int j = 0; j < N; ++j){
            if(!m_visited[j] && dist[j] < mincost){
                mincost = dist[j];
                index = j;
            }
        }

        m_visited[index] = true;
        sum += mincost;
        for(int j = 0; j < N; ++j){
            int s = graph[index][j];
            if(!m_visited[j] && dist[j] > graph[index][j]){
                dist[j] = graph[index][j];
            }
        }
    }
    return sum;
}
///////////////////////////////////////////////
bool cmp(node &a,node &b){
    return a.val < b.val;
}

int UnDirectedWeightAlgorithm::init(){
    int cnt = 0;
    for(int i = 0; i < N; ++i){
        for(int j = i; j < N; ++j){
            if(graph[i][j] != INF){
                m_edge[cnt].start = i;
                m_edge[cnt].end = j;
                m_edge[cnt].val = graph[i][j];
                cnt ++;
            }
        }
    }
    num_edge = cnt;
    std::sort(m_edge,m_edge + cnt,cmp);
}

void UnDirectedWeightAlgorithm::make_set(){
    for (int i = 0; i < N; ++i){
        m_father[i] = i;
        m_rank[i] = 1;
    }
    return;
}

int UnDirectedWeightAlgorithm::find_set(int x){
    if (x != m_father[x]){
        m_father[x] = find_set(m_father[x]);
    }
    return m_father[x];
}

void UnDirectedWeightAlgorithm::union_set(int x,int y){
    x = find_set(x);
    y = find_set(y);
    if(x == y)
        return;
    if(m_rank[x] < m_rank[y]){
        m_father[x] = find_set(y);
    }else{
        if(m_rank[x] == m_rank[y]){
            m_rank[x] ++;
        }
        m_father[y]  = find_set(x);
    }
    return;
}

int UnDirectedWeightAlgorithm::find_set_c(int x){
    int r = x;
    while(m_father[r] != r){
        r = m_father[r];
    }
    int i = x;
    int j = 0;
    while(m_father[i] != r){
        j = m_father[i];
        m_father[i] = r;
        i = j;
    }
    return r;
}

void UnDirectedWeightAlgorithm::union_set_c(int x,int y)
{
    int fx = find_set_c(x);
    int fy = find_set_c(y);
    if(fx != fy){
        m_father[fx] = fy;
    }
}

int UnDirectedWeightAlgorithm::Kruskal(int cur) {
    int sum = 0;
    make_set();
    for(int i = 0; i < num_edge; ++i){
        if(find_set_c(m_edge[i].start) != find_set_c(m_edge[i].end)){
            union_set_c(m_edge[i].start,m_edge[i].end);
            sum += m_edge[i].val;
        }
    }
    return sum;
}
