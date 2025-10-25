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
int find_best_solutioin(int i, vector<vector<int>> &adj, vector<Node> &nodes, int A, int B);
int graph_search (vector<vector<int>> &adj, vector<Node> &nodes, int A, int B);
int calculate_score(vector<vector<int>> &adj, vector<Node> &nodes);

int counter = 0;
int main() {
    int N, M, A, B;
    if (!read_input_args(N, M, A, B)) return 1;
    
    vector<vector<int>> adj(N + 1);
    vector<Node> nodes(N + 1);
    read_matrix(M, adj, nodes);
    //do i really need this 
    for (int i = 1; i <= N; ++i)
        nodes[i].id = i;

    sort (nodes.begin() + 1, nodes.end(), [](const Node &a, const Node &b)
        {return a.degree > b.degree;});
 
    // cout << adj[3].size() << ' ' << nodes[in].id << nodes[in].degree  << '\n';
    int best_score = graph_search(adj, nodes, A, B);
    cout << best_score << endl;
    return 0;
}

int graph_search (vector<vector<int>> &adj, vector<Node> &nodes, int A, int B){
    int best_score=0;
    // for(int i = 0; i <= V + 1; i++){
    best_score = find_best_solutioin(1, adj, nodes, A, B);
    // }
    //  cout << counter<< endl;

    return best_score;
}

int find_best_solutioin(int i, vector<vector<int>> &adj, vector<Node> &nodes, int A, int B){
    int best_score = 0;// A - extravert , B - introvert 
    int V = adj.size()-1;//amount of vertices 

    counter++;

    if(i > V || (A == 0 && B == 0)){
        best_score = calculate_score(adj, nodes);
        // cout << best_score << endl;
        return best_score;
    } 
    
    if(A != 0){
        nodes[i].agent_type = 1; 
        nodes[i].occupied=true; 
        best_score = max(best_score, find_best_solutioin(i + 1, adj, nodes, A - 1, B)); 
        nodes[i].agent_type = 0;
        nodes[i].occupied=false; 
    }

    if(B != 0){
        nodes[i].agent_type = 2; 
        nodes[i].occupied=true; 
        best_score = max(best_score, find_best_solutioin(i + 1,adj, nodes, A, B-1));  
        nodes[i].agent_type = 0;
        nodes[i].occupied=false; 
    }

    best_score = max(best_score, find_best_solutioin(i + 1, adj, nodes, A, B));

    return best_score;
}

int calculate_score(vector<vector<int>> &adj, vector<Node> &nodes){
    int score=0;
    for(int u=1; u <= nodes.size(); u++){
        if(!nodes[u].occupied) continue;

        for(int v : adj[u]){
 
            if(nodes[u].agent_type == 1 && (nodes[v].occupied)) score++;
            if(nodes[u].agent_type == 2 && (!nodes[v].occupied)) score++;
 
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
    // cout << "Read: " << N << " " << M << " " << A << " " << B << endl;

    bool valid = in_range(N, M, A, B);
    if  (valid != true ){
        // cout << "Invalid input" << endl;
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
    // cout << "reading" << N << M << A << B << endl;
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