#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

class DSU {
public:
    explicit DSU(int size): parent(vector<int>(size + 1, -1)), size(size){}

    int findSet(int element) {
        if (parent[element] < 0)
            return element;
        else {
            parent[element] = findSet(parent[element]);
            return parent[element];
        }
    }

    void Union(int element1, int element2) {
        int t1 = findSet(element1);
        int t2 = findSet(element2);
        if (t1 == t2)
            return;
        if (parent[t1] < parent[t2]) {
            swap(element1, element2);
            swap(t1, t2);
        }
        parent[t2] += parent[t1];
        parent[t1] = t2;
        size -= 1;
    }

    int Size() const {
        return size;
    }

private:
    vector<int> parent;
    int size;
};

struct Edge {
    int i, j, num;
    long long c;
    Edge(int i, int j, long long c, int n): i(i), j(j), c(c), num(n){}
    bool operator<(const Edge& other) const {
        return c < other.c;
    }
};

int n, m;
long long record = 0;
vector <Edge> c_i_j;
vector <vector<int>> result;
vector<int> last_res;
long long kruskal(const vector<bool>& is_blocked, const vector<int>& builded) {
    last_res.resize(0);
    long long cost = 0;
    DSU dsu(n);
    for (auto t : builded) {
        cost += c_i_j[t].c;
        if (record!= 0 && cost > record)
            return -1;
        dsu.Union(c_i_j[t].i, c_i_j[t].j);
    }
    for (auto t = 0; t < m; t++) {
        if (is_blocked[t] == false && dsu.findSet(c_i_j[t].i) != dsu.findSet(c_i_j[t].j)) {
            cost += c_i_j[t].c;
            if (record != 0 && cost > record)
                return -1;
            last_res.push_back(t);
            dsu.Union(c_i_j[t].i, c_i_j[t].j);
        }
    }
    //если size != 1 то выбрано > ребер чем нужно -> дерево не построить
    if (dsu.Size() == 1 && (cost == record || record==0)) {
        result.emplace_back();
        for (const auto& t : builded)
            result.back().push_back(c_i_j[t].num);
        for (const auto& t : last_res)
            result.back().push_back(c_i_j[t].num);
        return cost;
    }
    return -1;
}
void try_to_rebuild(vector<bool> is_blocked, vector<int> builded, const vector<int> last_success) {
    if (last_success.size()) {
        is_blocked[last_success[0]] = true;
        if (kruskal(is_blocked, builded) == record)
            try_to_rebuild(is_blocked, builded, last_res);
    }
    for (auto t = 1; t < last_success.size(); t++) {
        builded.push_back(last_success[t - 1]);
        is_blocked[last_success[t - 1]] = false;
        is_blocked[last_success[t]] = true;
        if (kruskal(is_blocked, builded) == record)
            try_to_rebuild(is_blocked, builded, last_res);
    }
}
void solve() {
    sort(c_i_j.begin(), c_i_j.end());
    auto is_blocked = vector<bool>(m, false);
    auto builded = vector<int>();
    record = kruskal(is_blocked, builded);
    remove_if(c_i_j.begin(), c_i_j.end(), [](Edge i) {return i.c > record; });
    try_to_rebuild(is_blocked, builded, last_res);
}
int main() {
    ifstream inputFile("input.txt", ios_base::in);
    ofstream outputFile("output.txt", ios_base::out);
    inputFile >> n >> m;
    int i, j;
    long long c;
    for (auto t = 0; t < m; t++) {
        inputFile >> i >> j >> c;
        c_i_j.push_back(Edge(i, j, c, t));
    }
    solve();
    outputFile << result.size() << "\n";
    for (const auto& mst : result) {
        for (const auto& a : mst) {
            outputFile << a + 1<< " ";
        }
        outputFile << "\n";
    }
    inputFile.close();
    outputFile.close();
    return 0;
}
