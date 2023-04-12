#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
typedef long long ll;
using namespace std;
ll n_building, m_refuge;
vector<ll> buildings;
vector<ll> refuges;
vector<pair<ll, ll>> buildings_num;
vector<pair<ll, ll>> refuges_num;
vector<vector<ll>> plan;
vector<ll> parent;
vector<ll> dist;
vector<ll> cycle;
auto get_time = [](ll x, ll y, ll p, ll q) {return abs(x - p) + abs(y - q) + 1; };
struct Edge {
    ll from;
    ll to;
    ll throughput;
    ll cost;
    ll flow;
    Edge(ll f, ll t, ll th, ll c, ll fl) : from(f), to(t), throughput(th), cost(c), flow(fl) {}
};
//s in 0,0; t in 0 n_building + m_refuge + 1
vector<vector<ll>> network;
vector<Edge> edges;
vector<ll> ref_sum;
void build_network() {
    //s with buildings
    for (auto i = 1; i < buildings_num.size(); i++) {
        auto b = buildings_num[i];
        network[0].push_back(edges.size());
        edges.push_back({ 0, i, buildings[i], 0, buildings[i] });
        network[i].push_back(edges.size());
        edges.push_back({ i, 0, 0, 0, -1* buildings[i] });
    }
    //buildings with refuses
    for (auto i = 1; i < buildings_num.size(); i++) {
        auto b = buildings_num[i];
        ll x = b.first, y = b.second;
        for (auto j = 1; j < refuges_num.size(); j++) {
            auto r = refuges_num[j];
            ll p = r.first, q = r.second;
            network[i].push_back(edges.size());
            edges.push_back({ i, j + n_building, INT_MAX, get_time(x,y,p,q), plan[i][j] });
            network[j + n_building].push_back(edges.size());
            edges.push_back({ j + n_building, i, 0, -1 * get_time(x,y,p,q), -1 * plan[i][j] });
        }
    }
    //refuses with t
    for (auto i = 1; i < refuges_num.size(); i++) {
        auto r = refuges_num[i];
        network[i + n_building].push_back(edges.size());
        edges.push_back({ i + n_building, n_building + m_refuge + 1, refuges[i], 0, ref_sum[i]});
        network[n_building + m_refuge + 1].push_back(edges.size());
        edges.push_back({ n_building + m_refuge + 1, i + n_building, 0, 0, -1*ref_sum[i]});
    }
}

long long flow_cost() {
    long long sum = 0;
    for (auto i = 0; i < edges.size(); i++) {
        sum += edges[i].cost * edges[i].flow;
    }
    return sum / 2;
}

ll find_cycle() {
    dist.assign(network.size(), 0);
    parent.assign(network.size(), -1);
    for (auto i = 0; i < network.size(); i++) {
        for (auto e = 0; e < edges.size(); e++) {
            ll v = edges[e].from, u = edges[e].to, c = edges[e].cost;
            if (edges[e].throughput == edges[e].flow)
                continue;
            if (dist[u] > dist[v] + c) {
                dist[u] = dist[v] + c;
                parent[u] = e;
                if (i + 1 == network.size())
                    return u;
            }
        }
    }
    return INT_MIN;
}

void delete_cycle(ll start) {
    for (auto i = 0; i < network.size(); i++)
        start = edges[parent[start]].from;
    ll e, v = start;
    while (true) {
        e = parent[v];
        cycle.push_back(e);
        v = edges[e].from;
        if (v == start)
            break;
    }
    ll min_cap = edges[cycle[0]].throughput - edges[cycle[0]].flow;
    for (const auto& v : cycle) {
        min_cap = min(min_cap, edges[v].throughput - edges[v].flow);
    }
    for (const auto& v : cycle) {
        edges[v].flow += min_cap;
        edges[v ^ 1].flow -= min_cap;
        if (edges[v].from <= n_building) {
            plan[edges[v].from][edges[v].to - n_building] += min_cap;
        }
        if (edges[v ^ 1].from <= n_building) {
            plan[edges[v ^ 1].from][edges[v ^ 1].to - n_building] -= min_cap;
        }
    }
}
int main() {
    ifstream inputFile("evacuate.in", ios_base::in);
    ofstream outputFile("evacuate.out", ios_base::out);
    inputFile >> n_building >> m_refuge;
    buildings_num.resize(n_building + 1);
    refuges_num.resize(m_refuge + 1);
    buildings.resize(n_building + 1);
    refuges.resize(m_refuge + 1);
    ref_sum.resize(m_refuge + 1);
    plan.resize(n_building + 1, vector<ll>(m_refuge + 1, 0));
    network.resize(m_refuge + n_building + 2);
    ll x, y, b;
    for (auto i = 1; i < n_building + 1; i++) {
        inputFile >> x >> y >> b;
        buildings_num[i] = { x, y };
        buildings[i] = b;
    }
    for (auto i = 1; i < m_refuge + 1; i++) {
        inputFile >> x >> y >> b;
        refuges_num[i] = { x, y };
        refuges[i] = b;
    }
    for (auto i = 1; i < n_building + 1; i++) {
        for (auto j = 1; j < m_refuge + 1; j++) {
            inputFile >> b;
            plan[i][j] = b;
            ref_sum[j] += plan[i][j];
        }
    }
    build_network();
    ll u = find_cycle();
    if (u == INT_MIN)
        outputFile << "OPTIMAL";
    else {
        outputFile << "SUBOPTIMAL\n";
        delete_cycle(u);
        for (auto i = 1; i < n_building + 1; i++) {
            for (auto j = 1; j < m_refuge + 1; j++) {
                outputFile << plan[i][j] << " ";
            }
            outputFile << '\n';
        }
    }
    return 0;
}
