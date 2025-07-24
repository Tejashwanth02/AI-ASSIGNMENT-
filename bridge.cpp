
#include <iostream>
#include <queue>
#include <unordered_set>
#include <vector>
#include <set>
using namespace std;

struct State {
    set<string> left;
    set<string> right;
    bool isUmbrellaLeft;
    int timeSpent;
    vector<string> path;

    string encode() const {
        string state;
        for (const string& p : left) state += p + ",";
        state += isUmbrellaLeft ? "|L" : "|R";
        return state;
    }
};

int getTime(const string& a, const string& b) {
    unordered_map<string, int> times = {
        {"Amogh", 5}, {"Ameya", 10}, {"GM", 20}, {"GF", 25}
    };
    return max(times[a], times[b]);
}

void BFS() {
    cout << "\nBFS Solution:\n";
    State start = {{"Amogh", "Ameya", "GM", "GF"}, {}, true, 0, {}};
    queue<State> q;
    unordered_set<string> visited;

    q.push(start);
    visited.insert(start.encode());

    while (!q.empty()) {
        State curr = q.front();
        q.pop();

        if (curr.left.empty() && !curr.isUmbrellaLeft && curr.timeSpent <= 60) {
            for (string step : curr.path) cout << step << "\n";
            cout << "Total time: " << curr.timeSpent << " minutes\n";
            return;
        }

        if (curr.timeSpent > 60) continue;

        if (curr.isUmbrellaLeft) {
            vector<string> people(curr.left.begin(), curr.left.end());
            int n = people.size();
            for (int i = 0; i < n; ++i) {
                for (int j = i; j < n; ++j) {
                    State next = curr;
                    string p1 = people[i], p2 = people[j];
                    next.left.erase(p1);
                    next.left.erase(p2);
                    next.right.insert(p1);
                    next.right.insert(p2);
                    next.isUmbrellaLeft = false;
                    int t = getTime(p1, p2);
                    next.timeSpent += t;
                    next.path.push_back("-> " + p1 + " and " + p2 + " cross (" + to_string(t) + " mins)");
                    string code = next.encode();
                    if (!visited.count(code)) {
                        visited.insert(code);
                        q.push(next);
                    }
                }
            }
        } else {
            vector<string> people(curr.right.begin(), curr.right.end());
            for (const string& p : people) {
                State next = curr;
                next.right.erase(p);
                next.left.insert(p);
                next.isUmbrellaLeft = true;
                int t = getTime(p, p);
                next.timeSpent += t;
                next.path.push_back("<- " + p + " returns (" + to_string(t) + " mins)");
                string code = next.encode();
                if (!visited.count(code)) {
                    visited.insert(code);
                    q.push(next);
                }
            }
        }
    }

    cout << "No solution found.\n";
}

int main() {
    BFS();
    return 0;
}
