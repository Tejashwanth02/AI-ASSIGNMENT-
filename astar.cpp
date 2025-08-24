#include <bits/stdc++.h>
using namespace std;//a*

int dx[8] = {-1,-1,-1,0,0,1,1,1};
int dy[8] = {-1,0,1,-1,1,-1,0,1};

struct Node {
    int x, y, g, h;
    vector<pair<int,int>> path;
    int f() const { return g + h; }
};

int heuristic(int x, int y, int n) {
    return abs(x - (n-1)) + abs(y - (n-1));
}

bool isValid(int x, int y, int n, vector<vector<int>>& grid) {
    return (x >= 0 && y >= 0 && x < n && y < n && grid[x][y] == 0);
}

pair<int, vector<pair<int,int>>> aStarSearch(vector<vector<int>> grid) {
    int n = grid.size();
    if (grid[0][0] == 1 || grid[n-1][n-1] == 1) return {-1, {}};

    priority_queue<pair<int, Node>, vector<pair<int, Node>>, greater<>> pq;
    Node start = {0,0,1,heuristic(0,0,n),{{0,0}}};
    pq.push({start.f(), start});

    vector<vector<int>> cost(n, vector<int>(n, INT_MAX));
    cost[0][0] = 1;

    while (!pq.empty()) {
        auto [_, node] = pq.top(); pq.pop();
        if (node.x == n-1 && node.y == n-1) return {node.g, node.path};

        for (int d=0; d<8; d++) {
            int nx = node.x + dx[d], ny = node.y + dy[d];
            if (isValid(nx, ny, n, grid)) {
                int newCost = node.g + 1;
                if (newCost < cost[nx][ny]) {
                    cost[nx][ny] = newCost;
                    Node next = {nx, ny, newCost, heuristic(nx,ny,n), node.path};
                    next.path.push_back({nx,ny});
                    pq.push({next.f(), next});
                }
            }
        }
    }
    return {-1, {}};
}

int main() {
    int n;
    cout << "Enter grid size n: ";
    cin >> n;

    vector<vector<int>> grid(n, vector<int>(n));
    cout << "Enter grid (0 for open, 1 for blocked):\n";
    for (int i=0; i<n; i++)
        for (int j=0; j<n; j++)
            cin >> grid[i][j];

    auto astar = aStarSearch(grid);

    cout << "A* Search  -> Path length: " << astar.first;
    if (astar.first != -1) {
        cout << ", Path: ";
        for (auto &p: astar.second) cout << "("<<p.first<<","<<p.second<<") ";
    }
    cout << "\n";
    return 0;
}
