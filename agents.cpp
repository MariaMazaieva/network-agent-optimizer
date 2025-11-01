#include <iostream>
#include <vector>
#include <bits/stdc++.h>

/*
    N - vertices, M - n edges, A - T1(extravert), B - T2(introvert) 
    Indexes in vector<Node> nodes (N); starts from  0 

*/

using namespace std;
enum AgentType {
    EMPTY = 0,       // Type: empty
    EXTRAVERT = 1,  // Type: A
    INTROVERT = 2, // Type: B
    CHECKED = 3   // Type: specifically left empty, skipped
};
struct Node{
    int id;
    int degree;
    bool occupied;
    AgentType agent_type;
    Node(int id = 0, int d = 0, bool occ = false, AgentType type = EMPTY) : 
        id(id), degree(d), occupied(occ), agent_type(type) {}

};

int global_best_score = 0;
int N, M, A, B;
vector<vector<int>> adj;

bool read_input_args();
bool in_range();
int read_matrix( vector<Node> &nodes);
void print_nodes(const vector<Node> &nodes);
void print_adj_list();

void find_best_solution(int i,  vector<Node> &nodes, int A, int B);
int remaining_max (int i_cur_node, vector<Node> &nodes, int restA, int restB);
int count_max_node_score(int i, vector<Node> &nodes, int restA, int restB);
int calculate_score( vector<Node> &nodes);

bool is_occupied (int i, vector<Node> &nodes);


int main() {
    if (!read_input_args()) return 1;
    adj.resize(N);
    vector<Node> nodes(N);
    read_matrix(nodes);

    // sort (nodes.begin(), nodes.end(), [](const Node &a, const Node &b)
    //     {return a.degree > b.degree;});

    find_best_solution(0, nodes, A, B);

    cout << global_best_score << endl;
    return 0;
}

void find_best_solution(int i, vector<Node> &nodes, int A, int B){
   
    if(i >= N || (A == 0 && B == 0)){
        int score = calculate_score(nodes);
        global_best_score = max(global_best_score, score);
        return ;
    } 
    
    int rest = remaining_max(i, nodes, A, B);
    if (rest < global_best_score)
        return ;

    if(A > 0 ){
        nodes[i].agent_type = EXTRAVERT; 
        nodes[i].occupied=true; 
        
        find_best_solution(i + 1, nodes, A - 1, B); 
        nodes[i].agent_type = EMPTY;
        nodes[i].occupied=false; 
    }

    if(B > 0){
        nodes[i].agent_type = INTROVERT; 
        nodes[i].occupied=true; 

        find_best_solution(i + 1, nodes, A, B-1);  
        nodes[i].agent_type = EMPTY;
        nodes[i].occupied=false; 
    }

    if( N - i  > A + B ){
        nodes[i].agent_type = CHECKED; 
        find_best_solution(i + 1, nodes, A, B);
        nodes[i].agent_type = EMPTY;
    }
}


int remaining_max (int i_cur_node, vector<Node>& nodes, int restA, int restB){
    int r_max = 0;
    int temp_score = 0;
  
    for( int i = 0; i < i_cur_node; i ++){
        if(nodes[i].agent_type == EXTRAVERT || nodes[i].agent_type == INTROVERT){
            temp_score = count_max_node_score(i, nodes, restA, restB);
            r_max += temp_score;
        }
    }

    int potential_extravert = 0, potential_introvert = 0;
    int sum_agents = restA + restB;
    for ( int v = i_cur_node; v < N; v ++){
        if (nodes[v].agent_type == EMPTY && ((sum_agents) > 0)){
            
            if(restA > 0){
                nodes[v].agent_type = EXTRAVERT;
                potential_extravert = count_max_node_score(v, nodes, restA-1, restB);
                // restA--;
            }
            if(restB > 0){
                nodes[v].agent_type = INTROVERT;
                potential_introvert = count_max_node_score(v, nodes, restA, restB-1);
                // restB--
            }

            if (potential_extravert >= potential_introvert){
                r_max += potential_extravert;
                restA--;
            }else{     
                r_max += potential_introvert;
                restB--;
            }
            sum_agents = restA + restB;
            nodes[v].agent_type = EMPTY;
        }
    }
    return r_max;
}

//!(is_occupied(v, nodes) = nodes[u].agent_type == CHECKED || nodes[u].agent_type == EMPTY)
int calculate_score( vector<Node> &nodes){
    int score=0;
    for(int u=0; u < N; u++){
        if(!is_occupied(u, nodes)) continue;

        for(int v : adj[u]){
            // Each agent scores based on their own perspective
            if (nodes[u].agent_type == EXTRAVERT && is_occupied(v, nodes)) 
                score++;
            
            else if (nodes[u].agent_type == INTROVERT && (!is_occupied(v, nodes)))
                score++;
        }
    }
    return score;
}

int count_max_node_score(int i, vector<Node> &nodes, int restA, int restB){
    int score = 0;
    int sum_agents = restA + restB;
    int remain = N - (A - restA) - (B - restB);//reamining free vertices

    for (int adj_u : adj[i]) {
        if (nodes[i].agent_type == EXTRAVERT) {

            if (nodes[adj_u].agent_type == EMPTY && sum_agents > 0) 
                {score += 1; sum_agents--; }
            
            else if (is_occupied(adj_u, nodes)) 
                score += 1;
        }
        else if (nodes[i].agent_type == INTROVERT) { 

            if (nodes[adj_u].agent_type == EMPTY && remain > sum_agents) 

                {score += 1; remain --;}
            
            else if (nodes[adj_u].agent_type == CHECKED)
                score++;
        }
    }
    return score;
}

bool is_occupied (int i, vector<Node> &nodes){
    if( nodes[i].agent_type == EXTRAVERT || 
        nodes[i].agent_type == INTROVERT)
        return true ;
    return false; 
}

int read_matrix( vector<Node> &nodes){
    int u, v;
    for (int i = 0; i < M; ++i) {
        cin >> u >> v;
        nodes[u-1].degree++;
        nodes[v-1].degree++;
        
        adj[u-1].push_back(v-1);
        adj[v-1].push_back(u-1);
        }
    return 0;
}

bool read_input_args(){
    cin >> N >> M >> A >> B;
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


void print_nodes(const vector<Node> &nodes) {
    cout << "----------------------------------------\n";
    cout << "Node list:\n";
    cout << "ID\tDegree\tAgentType\tOccupied\n";
    cout << "----------------------------------------\n";
    for (size_t i = 0; i < nodes.size() -1 ; ++i) {
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
    for (size_t i = 0; i < adj.size() - 1; ++i) {
        cout << i << " -> ";
        for (size_t j = 0; j < adj[i].size(); ++j) {
            cout << adj[i][j];
            if (j + 1 < adj[i].size()) cout << ", ";
        }
        cout << "\n";
    }

    cout << "----------------------\n";
}



