#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <fstream>

#include "Snap.h"

int present(std::vector<int> m , int n){
	std::vector<int>::iterator it;
	it=std::find(m.begin(),m.end(),n);
	if(it!=m.end())
		return 1;
	else	return 0;
}

bool IsNode(PUNGraph g,int v){
	TUNGraph::TNodeI NI = g->BegNI();
	for(;NI<g->EndNI();NI++){
		if(NI.GetId()==v)
			return true;
	}
	return false;
}

TUNGraph::TNodeI nodepointer(PUNGraph g, int d){
	TUNGraph::TNodeI NI = g->BegNI();
	while(d>0){
		NI++;
		d--;
	}
	return NI;
}

int main(){
	int n=10000;
	int m=900000;
	int samp_size=1000;
	int u,v,s_s,start,flag=0;
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
	float avg_deg_g = (2.0*g->GetEdges())/g->GetNodes();
	std::cout << "average-degree: " << avg_deg_g << std::endl;
	PUNGraph g_s;
	TUNGraph::TNodeI NJ;
	TUNGraph::TNodeI NI;
	std::vector<int> node_list;
	std::ofstream ofile;
	ofile.open("snow_ball_average_degree");
	//snowball sampling--
	int iterations = 1000;
	while(iterations > 0){
		s_s = samp_size;
		flag = 0;
		g_s=TUNGraph::New();
		start = TInt::GetRnd(n-1);	
		NI = nodepointer(g,start);	
		g_s->AddNode(start);
		s_s--;
		for (int e = 0; e < NI.GetOutDeg(); e++){
			u = NI.GetOutNId(e);
			g_s->AddNode(u);
			g_s->AddEdge(start,u);
			node_list.push_back(u);
			s_s--;
		}
		node_list.erase(node_list.begin());
		while(s_s>0){
			for(int i=0;i<(int)(node_list.size());i++){
				NI = nodepointer(g,node_list[i]);
				u = NI.GetId();
				for (int e = 0; e < NI.GetOutDeg(); e++){
					v = NI.GetOutNId(e);
					NJ = nodepointer(g,v);
					if((!IsNode(g_s,v))&&(!present(node_list,v))){
						node_list.push_back(v);
						g_s->AddNode(v);
						s_s--;
					}
					if(!g_s->IsEdge(u,v)){
						g_s->AddEdge(u,v);
					}
					if(s_s==0){
						flag=1;
						break;
					}
				}
				if(flag==1)
					break;	
				node_list.erase(node_list.begin());
			}	
		}
		printf("Graph has %d nodes and %d edges.\n", g_s->GetNodes(), g_s->GetEdges());
		float avg_deg_g_s = (2.0*g_s->GetEdges())/g_s->GetNodes();
		ofile << avg_deg_g_s << "\n";
		node_list.clear();
		iterations--;
	}
	ofile.close();
	return 0;
}
