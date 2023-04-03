#include <iostream>
#include <fstream>
#include <vector>
#include <climits>
using namespace std;
typedef long long ll;
vector<ll> mx, mn, sm, modifAdd, modifAssign;
ll n;
void ModifyAdd(ll v, ll l, ll r) {
    if (modifAssign[v] != LLONG_MAX) {
        if (v * 2 + 1 < 4 * n + 4) {
            modifAdd[v * 2] = 0;
            modifAdd[v * 2 + 1] = 0;
            modifAssign[v * 2] = modifAssign[v];
            modifAssign[v * 2 + 1] = modifAssign[v];
        }
        ll m = (l + r) / 2;
        sm[v] = (r - l + 1) * modifAssign[v];
        mn[v] = modifAssign[v];
        mx[v] = modifAssign[v];
        modifAssign[v] = LLONG_MAX;
    }
}
void ModifyAssign(ll v, ll l, ll r) {
    if (modifAdd[v] != 0) {
        if (v * 2 + 1 < 4 * n + 4) {
            modifAdd[v * 2] += modifAdd[v];
            modifAdd[v * 2 + 1] += modifAdd[v];
        }
        ll m = (l + r) / 2;
        sm[v] += (r - l + 1) * modifAdd[v];
        mn[v] += modifAdd[v];
        mx[v] += modifAdd[v];
        modifAdd[v] = 0;
    }
}
void Modify(ll v, ll l, ll r) {
    ModifyAdd(v, l, r);
    ModifyAssign(v, l, r);
}
ll Sum(ll v, ll inner_l, ll inner_r, ll l, ll r) {
    Modify(v, l, r);
    if (inner_l <= l && r <= inner_r) {
        return sm[v];
    }
    if (r < inner_l || inner_r < l) {
        return 0;
    }
    ll m = (l + r) / 2;
    ll s1 = Sum(v * 2, inner_l, inner_r, l, m);
    ll s2 = Sum(v * 2 + 1,  inner_l, inner_r, m + 1, r);
    return s1 + s2;
}
ll Min(ll v, ll inner_l, ll inner_r, ll l, ll r) {
    Modify(v, l, r);
    if (inner_l <= l && r <= inner_r) {
        return mn[v];
    }
    if (r < inner_l || inner_r < l) {
        return LLONG_MAX;
    }
    ll m = (l + r) / 2;
    return min(Min(v * 2, inner_l, inner_r, l, m),
        Min(v * 2 + 1, inner_l, inner_r, m + 1, r));
}
ll Max(ll v, ll inner_l, ll inner_r, ll l, ll r) {
    Modify(v, l, r);
    if (inner_l <= l && r <= inner_r) {
        return mx[v];
    }
    if (r < inner_l || inner_r < l) {
        return LLONG_MIN;
    }
    ll m = (l + r) / 2;
    return max(Max(v * 2, inner_l, inner_r, l, m),
        Max(v * 2 + 1, inner_l, inner_r, m + 1, r));
}

void Assign(ll v, ll val, ll inner_l, ll inner_r, ll l, ll r) {
    Modify(v, l, r);
    if (inner_l <= l && r <= inner_r) {
        modifAssign[v] = val;
        Modify(v, l, r);
        return;
    }
    if (r < inner_l || inner_r < l) {
        return;
    }
    ll m = (l + r) / 2;
    Assign(v * 2, val, inner_l,inner_r, l, m);
    Assign(v * 2 + 1, val, inner_l, inner_r, m + 1, r);
    sm[v] = sm[2 * v] + sm[2 * v + 1];
    mn[v] = min(mn[2 * v], mn[2 * v + 1]);
    mx[v] = max(mx[2 * v], mx[2 * v + 1]);
}
void Add(ll v, ll val, ll inner_l, ll inner_r, ll l, ll r) {
    Modify(v, l, r);
    if (inner_l <= l && r <= inner_r) {
        modifAdd[v] = val;
        Modify(v, l, r);
        return;
    }
    if (r < inner_l || inner_r < l) {
        return;
    }
    ll m = (l + r) / 2;
    Add(v * 2, val, inner_l, inner_r, l, m);
    Add(v * 2 + 1, val, inner_l, inner_r, m + 1, r);
    sm[v] = sm[2 * v] + sm[2 * v + 1];
    mn[v] = min(mn[2 * v], mn[2 * v + 1]);
    mx[v] = max(mx[2 * v], mx[2 * v + 1]);
}
int main()
{
    ifstream inputFile("input.txt", ios_base::in);
    ofstream outputFile("output.txt", ios_base::out);
    inputFile >> n;
    mn.resize(4 * n+4, 0);
    mx.resize(4 * n+4, 0);
    sm.resize(4 * n+4, 0);
    modifAdd.resize(4 * n+4, 0);
    modifAssign.resize(4 * n + 4, LLONG_MAX);
    ll operation, v, val, a, b;
    bool br = false;
    while (inputFile >> operation) {
        switch (operation) {
        case 0:
            br = true;
            break;
        case 1:
            inputFile >> v >> val;
            Assign(1, val, v, v, 0, n-1);
            break;
        case 2:
            inputFile >> a >> b >> val;
            Add(1, val, a, b, 0, n-1);
            break;
        case 3:
            inputFile >> a >> b;
            outputFile << Sum(1, a, b, 0, n-1)<<endl;
            break;
        case 4:
            inputFile >> a >> b;
            outputFile << Min(1, a, b,0, n-1)<<endl;
            break;
        case 5:
            inputFile >> a >> b;
            outputFile << Max(1, a, b, 0, n-1)<<endl;
            break;
        }
        if (br)
            break;
    }
}
