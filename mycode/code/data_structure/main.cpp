#include <iostream>
#include "string.h"
#include "Prime-Kruskal.h"
#include "Floyd-Dijkstra.h"

int main() {
    UnDirectedWeightAlgorithm  oa;
    int path = oa.Prime(0);
    std::cout<<"Prime path : "<<path<<std::endl;
    path = oa.Kruskal(oa.getEdgeNum());
    std::cout<<"Kruskal path : "<<path<<std::endl;
    //
    DirectedWeightAlgorithm ob;
    int * pDist = ob.getDist();
    int * pPath = ob.getPath();
    int S = 0;
    //ob.Dijkstra(ob.getGraph(),pDist,pPath,S);
    int vertexNum = ob.getVertexNum();
    //int ** pDistS = (int **)malloc(sizeof(int *) * vertexNum);
    int ** pDS = new int *[vertexNum];
    for(int i = 0; i < vertexNum; ++i){
        pDS[i] = new int [vertexNum];
    }
    int **pPa = new int*[vertexNum];
    for(int i = 0; i < vertexNum; ++i){
        pPa[i] = new int[vertexNum];
    }
    ob.InitArray(pDS,vertexNum);
    ob.InitArray(pPa,vertexNum);
    ob.Floyd(ob.getGraph(),pDS,pPa,S);
    std::cout<<"minest path: "<<std::endl;
    for (int i = 0; i < ob.getVertexNum(); i++){
        if (i != S){
            //ob.PrintPath(pPa, i,S);
            //printf("    %d\n", pDist[i]);
            ob.PrintPath(pPa, S,i);
            printf("    %d\n",pDS[S][i]);
        }
    }
    return 0;
}
