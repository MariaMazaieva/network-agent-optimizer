#include <iostream>
#include <vector>
#include <bits/stdc++.h>

/*
    N - vertices, M - n edges, A - T1(extravert), B - T2(introvert) 
    Indexes in vector<Node> nodes (N + 1); starts from 1, not 0 

*/

using namespace std;
struct Node{
    int id;
    int degree;
    bool occupied;
    int agent_type;
    Node(int id = 0, int d = 0, bool occ = false, int type = 0) : 
        id(id), degree(d), occupied(occ), agent_type(type) {}

};


bool read_input_args(int &N, int &M, int &A, int &B);
bool in_range(int N, int M, int A, int B);
int read_matrix(int M, vector<vector<int>> &adj, vector<Node> &nodes);

int main() {
    int N, M, A, B;
    bool is_ok = true;
    if (!read_input_args(N, M, A, B)) return 1;
    
    vector<vector<int>> adj(N + 1);
    vector<Node> nodes(N + 1);
    read_matrix(M, adj, nodes);
    //do i really need this 
    for (int i = 1; i <= N; ++i)
        nodes[i].id = i;

    sort (nodes.begin() + 1, nodes.end(), [](const Node &a, const Node &b)
        {return a.degree > b.degree;});
    

 
    int in = 3;
    cout << adj[3].size() << ' ' << nodes[in].id << nodes[in].degree  << '\n';
    return 0;
}

int graph_search (vector<vector<int>> &adj, vector<Node> &nodes){
    int max = 0, A = 2, B = 2;
    int V = adj.size();//amount of vvertices 
    for(int i = 0; i <= V + 1; i++){
        find_best_solutioin(adj, nodes, A, B)
    }

}

int find_best_solutioin(vector<vector<int>> &adj, vector<Node> &nodes, int A, int B){
    int best_score = 0;// A - extravert , B - introvert 
    int V = adj.size();//amount of vvertices 
    int agent_a = A, agent_b = B;
    // if(agent_a == 0 && agent_b == 0) return 0;

    if(agent_a == 0 && agent_b == 0) return; 
    
    nodes[i].agent_type = 2; 
    nodes[i].occupied=true; 
    find_best_solutioin(i + 1, A - 1, B); 

    nodes[V-i].agent_type = 1; 
    nodes[V-i].occupied=true; 
    find_best_solutioin(i + 1, A, B-1); 


    //compute the score
                
    //backtrack
    return best_score;
}

int calculate_score(vector<vector<int>> &adj, vector<Node> &nodes){
    int score=0;
    for(int u=1; u <= nodes.size(); u++){
        if(!nodes[u].occupied) continue;
        
        for(int v : adj[u]){
            if (u < v){
                if(nodes[u].agent_type == 1 && (nodes[v].occupied)) score++;
                if(nodes[u].agent_type == 2 && (!nodes[v].occupied)) score++;
            }
        }    
    
    }
    return score;
}




int read_matrix(int M, vector<vector<int>> &adj, vector<Node> &nodes){
        int u, v;
        for (int i = 0; i < M; ++i) {
            cin >> u >> v;
            nodes[u].degree++;
            nodes[v].degree++;
            
            adj[u].push_back(v);
            adj[v].push_back(u);
            // cout << u << v << '\n';
        }
    return 0;
}



bool read_input_args(int &N, int &M, int &A, int &B){
    cin >> N >> M >> A >> B;
    bool valid = in_range(N, M, A, B);
    if  (!valid){
        cout << "Invalid input" << endl;
        return false;
    }
   return true;
}

bool in_range(int N, int M, int A, int B){
    if ((N <= 30) &&
        (M <= 180) &&
        (A >= 1) &&
        (B >= 1 ) &&
        (A + B <= N))
    {return true;}
    
    return false;
}



/*
For each node u:
    If node[u] has T1:
        for each v in adj[u]:
            if node[v] occupied → add +1 to score
    If node[u] has T2:
        for each v in adj[u]:
            if node[v] empty → add +1 to score



*/