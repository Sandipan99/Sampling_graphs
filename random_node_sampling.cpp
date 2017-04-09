#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
//using namespace std;
//Generate G(n,m) step by step

#include "Snap.h"

int present(std::vector<int> m , int n){
	std::vector<int>::iterator it;
	it=std::find(m.begin(),m.end(),n);
	if(it!=m.end())
		return 1;
	else	return 0;
}
void display(std::vector<int> m){
	std::vector<int>::iterator it;
	for(it=m.begin();it!=m.end();it++)
		std::cout << *it << "\t";
	std::cout << "\n";
}

int main(int argc, char* argv[]) {
	int n=10000;
	int m=900000;
	int s_s=1000;
	int cnt=0;
	PUNGraph g=TUNGraph::New();
	for (int i=0; i<n; ++i)
		g->AddNode(i);
	for (int i=0; i<m; ++i)
	{
		while (true)
		{
			int j=TInt::GetRnd(n);
			int k=TInt::GetRnd(n);
			if ((j!=k)&&(!g->IsEdge(j,k)))
			{
				g->AddEdge(j,k);
				break;
			}
		}
	}
	printf("Graph has %d nodes and %d edges.\n", g->GetNodes(), g->GetEdges());
	//std::cout << "clustering coefficient: " << TSnap::GetClustCf(g,-1);
	//return 0;
	/*
	std::ofstream ofile;
	ofile.open("result/original_network");
	for (TUNGraph::TNodeI NI = g->BegNI(); NI < g->EndNI(); NI++) {
   		//printf("node id %d with out-degree %d and in-degree %d\n",NI.GetId(), NI.GetOutDeg(), NI.GetInDeg());
		ofile << NI.GetOutDeg() << "\n";
	}
	ofile.close();*/
	std::ofstream ofile;
	ofile.open("avg_deg_random_node");
	int iterations=1000;
	int it_cnt=0;
	while(it_cnt<iterations){
		PUNGraph g_c=TUNGraph::New();
		std::vector<int> temp;	
		while(cnt<s_s){
		//	int r=rand()%n;
			int r=TInt::GetRnd(n);
			if(!present(temp,r)){
				temp.push_back(r);
				g_c->AddNode(r);
				cnt++;
			}
		}
		//display(temp);
		for (TUNGraph::TNodeI NI = g->BegNI(); NI < g->EndNI(); NI++) {
			if(present(temp,NI.GetId())){
				for (int e = 0; e < NI.GetOutDeg(); e++) {
					if(present(temp,NI.GetOutNId(e))){
					//	printf("edge (%d %d)\n", NI.GetId(), NI.GetOutNId(e));
						g_c->AddEdge(NI.GetId(),NI.GetOutNId(e));
					}	
				}
			}
		}
		float avg_deg_g_c = (2.0*g_c->GetEdges())/g_c->GetNodes();
		ofile << avg_deg_g_c << "\n";
		it_cnt++;
		std::cout << "number of iterations: " << it_cnt << "\n";
		cnt=0;
	}
	ofile.close();
    return 0;
}
