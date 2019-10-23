#include "Floyd-Dijkstra.h"

void DirectedWeightAlgorithm::init(){
    std::cout<<"please input vexNum & EdgeNum: "<<std::endl;
    scanf("%d %d", &m_VertexNum, &m_EdgeNum);

    m_dist = (int *)malloc(sizeof (int) * m_VertexNum);//初始化dist
    m_path = (int *)malloc(sizeof (int) * m_VertexNum);//初始化path
    m_Graph = CreateGraph(m_VertexNum);
    m_Graph->Ne = m_EdgeNum;
    Edge E = (Edge)malloc(sizeof(struct ENode ));

    std::cout<<"please input weight of edge, like ' 0 1 100 '"<<std::endl;
    for(int i = 0; i < m_Graph->Ne ; i++){
        scanf("%d %d %d", &E->V1 , &E->V2 , &E->Weight );
        InsertEdge(m_Graph, E);
    }//至此，图创建完成
}

MGraph DirectedWeightAlgorithm::CreateGraph(int VertexNum){
    MGraph Graph = (MGraph)malloc(sizeof(struct GNode ));
    Graph->Nv = VertexNum;
    Graph->Ne = 0;
    for (int i = 0; i < Graph->Nv ; i++){
        for (int j = 0; j < Graph->Nv ; j++ )
            Graph->G[i][j] = INFINITY;
    }

    return Graph;
}

void DirectedWeightAlgorithm::InsertEdge(MGraph Graph, Edge E){
    Graph->G[E->V1 ][E->V2 ] = E->Weight ;
    //若是无向图，则还要插入边<V2, V1>
    //Graph->G[E->V2 ][E->V1 ] = E->Weight ;
}

Vertex DirectedWeightAlgorithm::FindMinDist(MGraph Graph, int *dist, int *collected){
    //返回未被收录顶点中dist最小者
    Vertex MinV, V;
    int MinDist = INFINITY;
    for (V = 0; V < Graph->Nv ; V++){
        if (collected[V] == false && dist[V] < MinDist){//如果V未被收录并且dist[V]更小
            MinDist = dist[V];//更新最小距离
            MinV = V;//更新对应顶点
        }
    }
    if (MinDist < INFINITY)//若找到最小dist
        return MinV;
    else
        return -1;
}

bool DirectedWeightAlgorithm::Dijkstra(MGraph Graph, int *dist, int *path, Vertex S){
    //dist[V]=S到V的最短距离，dist[S]=0，path[V]=S到V路上经过的父顶点
    int collected[MaxVertexNum];
    Vertex V, W;
    //初始化：此处默认邻接矩阵中不存在的边用INFINITY
    for (V = 0; V < Graph->Nv ; V++){
        dist[V] = Graph->G[S][V];
        if (dist[V] < INFINITY)
            path[V] = S;//将S周围一圈的邻接点赋予父顶点S
        else
            path[V] = -1;
        collected[V] = false;
    }
    //先将源点S收入集合中
    dist[S] = 0;//path[S] = -1
    collected[S] = true;
    while (1){
        //V=未被收录顶点中dist最小者
        V = FindMinDist(Graph, dist, collected);
        if (V == -1)
            break;//算法结束
        collected[V] = true;
        for (W = 0; W < Graph->Nv ; W++){//对于图中每一个顶点W
            if (collected[W] == false && Graph->G[V][W] < INFINITY){
                //若W是V的邻接点并且W未被收录
                if (Graph->G[V][W] < 0)
                    return false;//若有负值圈，则不能正确解决，返回错误标记
                if (dist[V] + Graph->G[V][W] < dist[W]){
                    //该情况为，V进入集合S后可能影响另外一个W的dist，且只影响V周围一圈邻接点的dist值
                    dist[W] = dist[V] + Graph->G[V][W];//更新dist[W]
                    path[W] = V;//更新S到W的路径，即增加W的父顶点V
                }
            }
        }
    }
    return true;//算法执行完毕，返回正确标记
}

void DirectedWeightAlgorithm::PrintPath(int *path, Vertex W, Vertex S){
    //打印最短路径经过的所有顶点,此处打印0（path[0]=-1)到每个顶点的最短路径
    //先逆序push，再顺序pop
    std::stack<int> s;
    s.push(W);//path[W] = V,path[S] = -1
    while (W != S){
        W = path[W];
        s.push(W) ;
    }//W = S时跳出循环，即到达源点
    while (!s.empty() ){
        if (s.size() == 1 )
            printf("%d", s.top() );
        else
            printf("%d ", s.top() );
        s.pop() ;
    }
}

void DirectedWeightAlgorithm::PrintPath(int **path,Vertex S,Vertex W){
    std::stack<int> s;
    s.push(W);
    while(path[S][W] != INFINITY){
        W = path[S][W];
        s.push(W);
    }
    std::cout<<S<<" ";
    while (!s.empty() ){
        printf("%d ", s.top() );
        s.pop() ;
    }
}

void DirectedWeightAlgorithm::InitArray(int **p,int vertexNum){
    for(int i = 0; i < vertexNum; ++i){
        for(int j = 0; j < vertexNum; ++j){
            p[i][j] = INFINITY;
        }
    }
}

int DirectedWeightAlgorithm::Floyd(MGraph Graph,int ** dist,int **path,Vertex S){
    Vertex V;
    for (V = 0; V < Graph->Nv ; V++){
        dist[S][V] = Graph->G[S][V];
//        if (dist[S][V] < INFINITY)
//            path[V] = S;
//        else
//            path[V] = -1;
    }
    dist[S][V] = 0;
    for(int k = 0; k < Graph->Nv; ++k){
        for(int i = 0; i < Graph->Nv; ++i)
            for(int j = 0; j < Graph->Nv; ++j){
                if(Graph->G[i][j] > Graph->G[i][k] + Graph->G[k][j]){
                    path[i][j] = k;
                    Graph->G[i][j] = Graph->G[i][k] + Graph->G[k][j];
                    dist[i][j] = Graph->G[i][j];
                }
            }
    }

}
