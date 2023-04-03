#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <climits>
using namespace std;
typedef long long ll;
vector<ll> minimum_tree, modify;
ll n;
void Modify(ll v, ll l, ll r) {
	if (modify[v] != LLONG_MAX) {
		if (v * 2 + 1 < 4 * n) {
			modify[v * 2] = modify[v];
			modify[v * 2 + 1] = modify[v];
		}
		ll m = (l + r) / 2;
		minimum_tree[v] = modify[v];
		modify[v] = LLONG_MAX;
	}
}
ll Min(ll v, ll inner_l, ll inner_r, ll l, ll r) {
	Modify(v, l, r);
	if (inner_l <= l && r <= inner_r) {
		return minimum_tree[v];
	}
	if (r < inner_l || inner_r < l) {
		return LLONG_MAX;
	}
	ll m = (l + r) / 2;
	return min(Min(v * 2, inner_l, inner_r, l, m),
		Min(v * 2 + 1, inner_l, inner_r, m + 1, r));
}

void Assign(ll v, ll val, ll inner_l, ll inner_r, ll l, ll r) {
	Modify(v, l, r);
	if (inner_l <= l && r <= inner_r) {
		modify[v] = val;
		Modify(v, l, r);
		return;
	}
	if (r < inner_l || inner_r < l) {
		return;
	}
	ll m = (l + r) / 2;
	Assign(v * 2, val, inner_l, inner_r, l, m);
	Assign(v * 2 + 1, val, inner_l, inner_r, m + 1, r);
	minimum_tree[v] = min(minimum_tree[2 * v], minimum_tree[2 * v + 1]);
}

ll Min(ll left, ll right) {
	return Min(1, left, right, 0, n - 1);
}

void Assign(ll v, ll val) {
	Assign(1, val, v, v, 0, n - 1);
}

ll solve(vector<tuple<ll, ll, ll>>& results) {
	minimum_tree.resize(4 * n, LLONG_MAX);
	modify.resize(4 * n, LLONG_MAX);
	ll res = 1;
	sort(results.begin(), results.end());
	Assign(get<1>(results[0]) - 1, get<2>(results[0]));
	for (auto i = 1; i < n; i++) {
		if (get<2>(results[i]) < Min(0, get<1>(results[i]) - 1))
			res++;
		Assign(get<1>(results[i]) - 1, get<2>(results[i]));
	}
	return res;
}
int main()
{
    ifstream inputFile("input.txt", ios_base::in);
    ofstream outputFile("output.txt", ios_base::out);
    inputFile >> n;
    vector<tuple<ll, ll, ll>> results(n);
    for (auto i = 0; i < n; i++)
        inputFile >> get<0>(results[i]) >> get<1>(results[i]) >> get<2>(results[i]);

    outputFile << solve(results);
    inputFile.close();
    outputFile.close();
}
