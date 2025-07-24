//rabbit.cpp
#include <iostream>
#include <queue>
#include <stack>
#include <unordered_set>
#include <vector>
using namespace std;

string goal = "kuch";

vector<string> getNextStates(const string& state) {
    vector<string> nextStates;
    int pos = state.find('_');

    if (pos > 0 && state[pos - 1] == 'E') {
        string s = state;
        swap(s[pos], s[pos - 1]);
        nextStates.push_back(s);
    }

    if (pos < 6 && state[pos + 1] == 'W') {
        string s = state;
        swap(s[pos], s[pos + 1]);
        nextStates.push_back(s);
    }

    if (pos > 1 && state[pos - 2] == 'E') {
        string s = state;
        swap(s[pos], s[pos - 2]);
        nextStates.push_back(s);
    }

    if (pos < 5 && state[pos + 2] == 'W') {
        string s = state;
        swap(s[pos], s[pos + 2]);
        nextStates.push_back(s);
    }

    return nextStates;
}

void BFS(string start) {
    cout << "BFS Path:\n";
    queue<pair<string, vector<string>>> q;
    unordered_set<string> visited;

    q.push({start, {start}});
    visited.insert(start);

    while (!q.empty()) {
        auto [curr, path] = q.front();
        q.pop();

        if (curr == goal) {
            for (const string& s : path)
                cout << s << " -> ";
            cout << "GOAL\n";
            return;
        }

        for (const string& next : getNextStates(curr)) {
            if (!visited.count(next)) {
                visited.insert(next);
                vector<string> newPath = path;
                newPath.push_back(next);
                q.push({next, newPath});
            }
        }
    }

    cout << "No solution found.\n";
}

void DFS(string start) {
    cout << "\nDFS Path:\n";
    stack<pair<string, vector<string>>> stk;
    unordered_set<string> visited;

    stk.push({start, {start}});

    while (!stk.empty()) {
        auto [curr, path] = stk.top();
        stk.pop();

        if (curr == goal) {
            for (const string& s : path)
                cout << s << " -> ";
            cout << "GOAL\n";
            return;
        }

        if (visited.count(curr)) continue;
        visited.insert(curr);

        for (const string& next : getNextStates(curr)) {
            if (!visited.count(next)) {
                vector<string> newPath = path;
                newPath.push_back(next);
                stk.push({next, newPath});
            }
        }
    }

    cout << "No solution found.\n";
}

int main() {
    string start = "kich"; 
    BFS(start);
    DFS(start);
    return 0;
}
