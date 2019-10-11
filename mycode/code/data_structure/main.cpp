#include <iostream>
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
    ob.Dijkstra(ob.getGraph(),pDist,pPath,S);
    std::cout<<"minest path: "<<std::endl;
    for (int i = 0; i < ob.getVertexNum(); i++){
        if (i != S){
            ob.PrintPath(pPath, i, S);
            printf("    %d\n", pDist[i]);
        }
    }
    return 0;
}