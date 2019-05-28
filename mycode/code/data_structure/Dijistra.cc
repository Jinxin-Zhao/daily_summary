/* 
	Name: Dijkstra算法 
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
using namespace std;

//图的邻接矩阵表示法
#define MaxVertexNum 100 //最大顶点数设为100 
#define INFINITY 65535 //无穷设为双字节无符号整数的最大值65535 
typedef int Vertex;//用顶点下标表示顶点，为整型 
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

MGraph CreateGraph(int VertexNum)
//图的创建，并初始化一个具有VertexNum个顶点且Edge为0的空图
{
	MGraph Graph = (MGraph)malloc(sizeof(struct GNode ));
	Graph->Nv = VertexNum;
	Graph->Ne = 0;
	for (int i = 0; i < Graph->Nv ; i++){
		for (int j = 0; j < Graph->Nv ; j++ )
			Graph->G[i][j] = INFINITY;
	}
	
	return Graph;
 } 
 
void InsertEdge(MGraph Graph, Edge E)
//插入边 
{
	Graph->G[E->V1 ][E->V2 ] = E->Weight ;
	//若是无向图，则还要插入边<V2, V1>
	//Graph->G[E->V2 ][E->V1 ] = E->Weight ; 
}

Vertex FindMinDist(MGraph Graph, int *dist, int *collected)
//返回未被收录顶点中dist最小者 
{
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

bool Dijkstra(MGraph Graph, int *dist, int *path, Vertex S)
//dist[V]=S到V的最短距离，dist[S]=0，path[V]=S到V路上经过的父顶点 
{
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

void PrintPath(int *path, Vertex W, Vertex S)
//打印最短路径经过的所有顶点,此处打印0（path[0]=-1)到每个顶点的最短路径 
//先逆序push，再顺序pop 
{
	stack<int> s;
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

int main()
{
	int VertexNum, EdgeNum;
    std::cout<<"please input vexNum & EdgeNum: "<<std::endl;
	scanf("%d %d", &VertexNum, &EdgeNum);

	int *dist = (int *)malloc(sizeof (int) * VertexNum);//初始化dist 
	int *path = (int *)malloc(sizeof (int) * VertexNum);//初始化path 
	MGraph Graph = CreateGraph(VertexNum);
	Graph->Ne = EdgeNum;
	Edge E = (Edge)malloc(sizeof(struct ENode ));

    std::cout<<"please input weight of edge, like ' 0 1 100 '"<<std::endl;
	for(int i = 0; i < Graph->Ne ; i++){
		scanf("%d %d %d", &E->V1 , &E->V2 , &E->Weight );
		InsertEdge(Graph, E);
	}//至此，图创建完成
	//下面进入Dijkstra算法部分
	int S = 0;//定义源点S,Graph->G[0][0]=INFINITY
	Dijkstra(Graph, dist, path, S);
	//PrintPath(path);
    std::cout<<"minest path: "<<std::endl;
	for (int i = 0; i < VertexNum; i++){
		if (i != S){
			PrintPath(path, i, S);
			printf("    %d\n", dist[i]);
		}
	}
	
	return 0;
	 
}
