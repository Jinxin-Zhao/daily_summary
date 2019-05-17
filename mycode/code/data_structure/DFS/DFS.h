#include <iostream>
#include <queue>
#define MAXV 50
bool visited[MAXV];

typedef struct ANode{
        int adjvex;
        struct ANode * nextarc;
}ArcNode;

typedef struct Vnode{
    ArcNode * firstarc;
}VNode;

typedef VNode AdjList[MAXV];

typedef struct{
    AdjList adjlist;
    int n,e;
}ALGraph;

std::queue<int>  g_que;

void ArrayToList(int * Arr,int n,ALGraph ** G){
    int count=0;  //count用于统计边数，即矩阵中非0元素个数
    ArcNode *p;
    (*G) = (ALGraph *)malloc(sizeof(ALGraph));
    (*G)->n = n;
    for (int i = 0; i < n; i++)                 //给邻接表中所有头节点的指针域置初值
        (*G)->adjlist[i].firstarc = nullptr;
    for (int i = 0; i < n; i++)                 //检查邻接矩阵中每个元素
        for (int j = n-1; j >= 0; j--)
            if (Arr[i*n+j] != 0)      //存在一条边，将Arr看作n×n的二维数组，Arr[i*n+j]即是Arr[i][j]
            {
                count++;
                p = (ArcNode *)malloc(sizeof(ArcNode));   //创建一个节点*p
                p->adjvex = j;
                p->nextarc = (*G)->adjlist[i].firstarc;      //采用头插法插入*p
                (*G)->adjlist[i].firstarc=p;
            }
    (*G)->e = count;
}


void DispAdj(ALGraph *G)   //输出邻接表G
{
    int i;
    ArcNode *p;
    for (i=0; i<G->n; i++)
    {
        p=G->adjlist[i].firstarc;
        printf("%3d: ",i);
        while (p!=NULL)
        {
            printf("-->%d ",p->adjvex);
            p=p->nextarc;
        }
        printf("\n");
    }
}

//array version
//void DFS(ALGraph & G,int v){
//    visited[v]= true; //从V开始访问，flag它
//    printf("%d",v);    //打印出V
//    for(int j = 0;j < G.vexnum;j++)
//        if(G.arcs[v][j] == 1 && visited[j] == false) //这里可以获得V未访问过的邻接点
//            DFS(G,j); //递归调用，如果所有节点都被访问过，就回溯，而不再调用这里的DFS
//}

//Adjlist version
void DFS(ALGraph * G,int v){
    visited[v]= true; //从V开始访问，flag它
    printf("%3d",v);    //打印出V

    ArcNode * p = G->adjlist[v].firstarc;
    while(p != nullptr) {
        if(visited[p->adjvex] == false){
            DFS(G,p->adjvex);
        }
        p = p->nextarc;
    }
}

void DFSTraverse(ALGraph * G){
    for(int v = 0; v < G->n; v++){
        visited[v] = false;
    }
    std::cout<<"  start to DFStraverse Graph ......"<<std::endl;
    int v = 0;
    //for(int v = 0; v < G->n; v++){
        if(visited[v] == false){
            DFS(G,v);
       // }
    }

}

void BFS(ALGraph * G,int v){
    visited[v]= true; //从V开始访问，flag它
    printf("%3d",v);    //打印出V

    g_que.push(v);
    ArcNode * p = nullptr;
    while(!g_que.empty()){
        int vex = g_que.front();
        p = G->adjlist[vex].firstarc;
        g_que.pop();
        while(p != nullptr){
            if(visited[p->adjvex] == false){
                visited[p->adjvex] = true;
                printf("%3d",p->adjvex);
                g_que.push(p->adjvex);
            }
            p = p->nextarc;
            //p = G->adjlist[p->adjvex].firstarc;
        }
    }
}

void BFSTraverse(ALGraph * G){
    for(int v = 0; v < G->n; v++){
        visited[v] = false;
    }
    std::cout<<"  start to BFStraverse Graph ......"<<std::endl;
    int v = 0;
    //for(int v = 0; v < G->n; v++){
    if(visited[v] == false){
        BFS(G,v);
        // }
    }

}