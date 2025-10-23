#include <iostream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;
struct Node{
    int value;
    int degree;
    
    Node(int v = 0, int p = 0) : value(v), degree(p) {}

};


bool read_input_args(int &N, int &M, int &A, int &B);
bool in_range(int N, int M, int A, int B);
int read_matrix(int M, vector<Node> &nodes);

int main() {
    int N, M, A, B;
    bool is_ok = true;
    if (!read_input_args(N, M, A, B)) return 1;
    vector<Node> nodes(N + 1);

    read_matrix(M, nodes);
    return 0;
}


int read_matrix(int M, vector<Node> &nodes){
    int u, v;
    for (int i = 0; i < M; ++i) {
        cin >> u >> v;
        nodes[u].degree++;
        nodes[v].degree++;
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
