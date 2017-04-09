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
	float avg_deg_g = (2.0*g->GetEdges())/g->GetNodes();
	std::cout << "average-degree: " << avg_deg_g << std::endl;
	
	std::ofstream ofile;
	ofile.open("avg_deg_file_random_edge");
	int iterations=1000;
	int it_cnt=0,u,v;
	while(it_cnt<iterations){
		std::vector<int> edge;
		std::vector<int> node;
		PUNGraph g_c=TUNGraph::New();
		std::vector<int> sample;	
		while(cnt<s_s){
			int r=TInt::GetRnd(m);
			if(!present(sample,r)){
				sample.push_back(r);
				cnt++;
			}
		}
		//std::cout << "number of edges to sample: " << cnt << " " << sample.size() << std::endl;
		cnt = 0;
		for (TUNGraph::TNodeI NI = g->BegNI(); NI < g->EndNI(); NI++) {
   			for (int e = 0; e < NI.GetOutDeg(); e++) {
				//std::cout << "iterating edge: " << NI.GetId() << "->" << NI.GetOutNId(e);
				if(present(sample,cnt)){
					u = NI.GetId();
					v = NI.GetOutNId(e);
					edge.push_back(u);
					edge.push_back(v);
					if(!present(node,u))
						node.push_back(u);
					if(!present(node,v))
						node.push_back(v);
				}
				cnt++;
    			}
  		}		
		for(int i=0;i<(int)(node.size());i++)
			g_c->AddNode(node[i]);
		int j=0;
		while(j<(int)(edge.size())){
			g_c->AddEdge(edge[j],edge[j+1]);
			j+=2;
		}		
		printf("Graph has %d nodes and %d edges.\n", g_c->GetNodes(), g_c->GetEdges());
		float avg_deg_g_c = (2.0*g_c->GetEdges())/g_c->GetNodes();
		std::cout << "average-degree: " << avg_deg_g_c << std::endl;
		ofile << avg_deg_g_c << "\n";	
		std::cout << "number of iterations: " << it_cnt << "\n";
		cnt=0;
		
		it_cnt++;
	}
	ofile.close();

	return 0;
}


