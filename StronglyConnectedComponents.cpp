#include <bits/stdc++.h>
#define endl '\n'

using namespace std;

void DFS(vector<int> graph[], int u, vector<bool>& visited, stack<int>& stack) {
    visited[u] = true;
    for (int v : graph[u]) {
        if (!visited[v]) {
            DFS(graph, v, visited, stack);
        }
    }
    stack.push(u);
}

void DFS(vector<int> graph[], int u, vector<bool>& visited, vector<int>& ord) {
    visited[u] = true;
    ord.push_back(u);
    for (int v : graph[u]) {
        if (!visited[v]) {
            DFS(graph, v, visited, ord);
        }
    }
}

void Transpose(vector<int> graph[], int u, vector<int> transpose[]) {
    for (int i = 0; i < u; i++) {
        for (int j : graph[i]) {
            transpose[j].push_back(i);
        }
    }
}

void FindSCC(vector<int> graph[], int vertexNumber) {
    vector<bool> visited(vertexNumber, false);
    stack<int> stack;

    for (int vertex = 0; vertex < vertexNumber; vertex++) {
        if (!visited[vertex]) {
            DFS(graph, vertex, visited, stack);
        }
    }

    vector<int> transpose[vertexNumber];
    Transpose(graph, vertexNumber, transpose);

    fill(visited.begin(), visited.end(), false);
    vector<vector<int>> SCCs;
    while (!stack.empty()) {
        int vertex = stack.top();
        stack.pop();
        if (!visited[vertex]) {
            vector<int> ord;
            DFS(transpose, vertex, visited, ord);
            sort(ord.begin(), ord.end());
            SCCs.push_back(ord);
        }
    }

    sort(SCCs.begin(), SCCs.end());
    cout << SCCs.size() << '\n';
    for (vector<int>& ord : SCCs) {
        for (int vertex : ord) {
            cout << vertex;
            if(vertex != ord.back()) cout << " ";
        }
        cout << '\n';
    }
    cout << '\n';
}


int main(int argc, char *argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int C; cin >> C;

    for (int testCase = 0; testCase < C; testCase++) {
        int N; cin >> N;
        vector<int> graph[N];
        for (int vertex = 0; vertex < N; vertex++)
        {
            int index, edges; char colon;
            cin >> index >> edges >> colon;
            for (int edge = 0; edge < edges; edge++)
            {
                int edgeVertex; cin >> edgeVertex;
                graph[vertex].push_back(edgeVertex);
            }
        }

        FindSCC(graph, N);
    }
    


    return 0;
}
