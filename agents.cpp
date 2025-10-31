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

int counter = 0;
int global_best_score =0;
int N, M, A, B;

bool read_input_args();
bool in_range();
int read_matrix( vector<Node> &nodes);
int find_best_solution(int i,  vector<Node> &nodes, int A, int B);
int graph_search ( vector<Node> &nodes);
int calculate_score( vector<Node> &nodes);
int count_neighb_score(int i, vector<Node> &nodes);
void print_nodes(const vector<Node> &nodes);
void print_adj_list();
int remaining_max (vector<Node> nodes, int restA, int restB);
vector<Node> agent(N+1);

vector<vector<int>> adj;

int main() {
    if (!read_input_args()) return 1;
    adj.resize(N + 1);
    vector<Node> nodes(N + 1);
    read_matrix(nodes);
  
    //do i really need this 
    for (int i = 1; i <= N; ++i){
        nodes[i].id = i;
        nodes[i].occupied = false;
        nodes[i].agent_type = 0;
    }
    sort (nodes.begin() + 1, nodes.end(), [](const Node &a, const Node &b)
        {return a.degree > b.degree;});
    
    int best_score = graph_search(nodes);
    


    cout << global_best_score << endl;
    return 0;
}

int graph_search (vector<Node> &nodes){
    int best_score=0;
    
    int V = adj.size() - 1;
    vector<int> suffix_degree(V + 2, 0);  
    for (int j = V; j >= 1; --j) {
       suffix_degree[j] = suffix_degree[j + 1] + nodes[j].degree;
    } 
    cout << B << endl ;
    print_adj_list();
    best_score = find_best_solution(1, nodes, A, B);
    
    return best_score;
}

int find_best_solution(int i, vector<Node> &nodes, int A, int B){
    int V = nodes.size()-1; //amount of vertices 


   if(i > V || (A == 0 && B == 0)){

        int score = calculate_score( nodes);

        print_nodes(nodes);
        global_best_score = max(global_best_score, score);

        return 0;
    } 
    int count_neighbours =0;

    if(A > 0 ){
        nodes[i].agent_type = 1; 
        nodes[i].occupied=true; 
        // int score = count_neighb_score(i, adj, nodes);
        
        int rest = remaining_max(nodes, A - 1,B);
        if (rest < global_best_score)
            return 0;
        
        find_best_solution(i + 1, nodes, A - 1, B); 
        nodes[i].agent_type = 0;
        nodes[i].occupied=false; 
    }

    if(B > 0){
        nodes[i].agent_type = 2; 
        nodes[i].occupied=true; 
        // int temp_score = ca
        // int score = count_neighb_score(i, adj, nodes);
        int score = calculate_score( nodes);
   
        int rest = remaining_max(nodes, A, B - 1);
        if (rest < global_best_score)
            return 0;
        find_best_solution(i + 1, nodes, A, B-1);  
        nodes[i].agent_type = 0;
        nodes[i].occupied=false; 
    }

    if( V - i >= A + B ){
        nodes[i].agent_type = 3; // 3 - CHECKED
        find_best_solution(i + 1, nodes, A, B);
        nodes[i].agent_type = 0;
        // nodes[i].occupied=false; 
    }

    return 0;
}


int remaining_max (vector<Node> nodes, int restA, int restB){
    int r_max = 0;
    int score = calculate_score(nodes);
    int temp_score = 0;
    r_max = score;
    if (restA <= 0 && restB <= 0) return score;
    
    for( int i = 1; i <= N; i ++){
        if(nodes[i].agent_type == 1 || nodes[i].agent_type == 2){
            temp_score = (i, nodes);
        }

    }

    int new_score = calculate_score(nodes);

    if (new_score > score)
        r_max = new_score;

    return r_max;
}


int calculate_score( vector<Node> &nodes){
    int score=0;
    for(int u=1; u <= N; u++){
        if(!nodes[u].occupied) continue;

        for(int v : adj[u]){
            // Each agent scores based on their own perspective
            if (nodes[u].agent_type == 1 && nodes[v].occupied) score++;
            if (nodes[u].agent_type == 2 && !nodes[v].occupied) score++;
        }
    }
    return score;
}

int count_neighb_score(int i, vector<Node> nodes, int restA, int restB){
    int score = 0;
    int sum_agents = restA + restB;
    int remain = N - (A - restA) - (B - restB);//reamining free vertices

    for (int v : adj[i]) {
        if (nodes[i].agent_type == 1) {              
            if (nodes[v].agent_type == 0 && sum_agents > 0) 
                { score += 1; sum_agents--; }  
        }
        else if (nodes[i].agent_type == 2) { 
            if (nodes[v].agent_type == 0 && remain > restA + restB) 
                {score += 1; remain --;}
        }
    }
    return score;
}

int read_matrix( vector<Node> &nodes){
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

bool read_input_args(){
    cin >> N >> M >> A >> B;
    // cout << "Read: " << N << " " << M << " " << A << " " << B << endl;
    return in_range();
}

bool in_range(){
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


void print_nodes(const vector<Node> &nodes) {
    cout << "----------------------------------------\n";
    cout << "Node list:\n";
    cout << "ID\tDegree\tAgentType\tOccupied\n";
    cout << "----------------------------------------\n";
    for (size_t i = 1; i < nodes.size(); ++i) {
        cout << nodes[i].id << "\t"
             << nodes[i].degree << "\t"
             << nodes[i].agent_type << "\t\t"
             << (nodes[i].occupied ? "Yes" : "No") << "\n";
    }
    cout << "----------------------------------------\n";
}

void print_adj_list() {
    cout << "----------------------\n";
    cout << "Adjacency List:\n";
    cout << "----------------------\n";

    // Start from 1 because your graph is 1-indexed
    for (size_t i = 1; i < adj.size(); ++i) {
        cout << i << " -> ";
        for (size_t j = 0; j < adj[i].size(); ++j) {
            cout << adj[i][j];
            if (j + 1 < adj[i].size()) cout << ", ";
        }
        cout << "\n";
    }

    cout << "----------------------\n";
}



