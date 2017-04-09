#include<stdlib.h>
#include<vector>
#include<fstream>
#include<iostream>

#include "Snap.h"

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
	int u,v,s_s,start;
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
	TUNGraph::TNodeI NI;
	PUNGraph g_s;
	//random-walk sampling-----
	std::ofstream ofile;
	ofile.open("average_degree_random_walk_sampling");
	int iterations = 1000;
	while(iterations>0)
	{
		g_s=TUNGraph::New();
		std::vector<int> node_list;
		start = TInt::GetRnd(n-1);	
		NI = nodepointer(g,start);	
		g_s->AddNode(start);
		s_s = samp_size;
		s_s--;
		while(s_s>0){
			for (int e = 0; e < NI.GetOutDeg(); e++){
				u = NI.GetOutNId(e);
				node_list.push_back(u);
			}
			v = TInt::GetRnd((int)(node_list.size()));
			u = node_list[v];
			if(!IsNode(g_s,u))
				g_s->AddNode(u);
			//if (!g->IsEdge(start,u))
			//std::cout << "adding edge" << start << "->" << u << std::endl;
			g_s->AddEdge(start,u);	
			s_s--;
			start = u;
			NI = nodepointer(g,start);
			node_list.clear();
		}
		printf("Graph has %d nodes and %d edges.\n", g_s->GetNodes(), g_s->GetEdges());
		float avg_deg_g_s = (2.0*g_s->GetEdges())/g_s->GetNodes();
		std::cout << "average-degree: " << avg_deg_g_s << std::endl;
		ofile << avg_deg_g_s << "\n";
		iterations--;
	}
	return 0;
}
