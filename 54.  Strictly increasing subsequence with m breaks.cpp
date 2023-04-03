#include <iostream>
#include <fstream>
#include <vector>
#include <climits>
#include <algorithm>
#include <map>
typedef long long ll;
using namespace std;

ll GetMax(ll l, ll r, ll j, vector<vector<ll>>& dp) {
	ll result = LLONG_MIN;
	for (; r >= l; r = (r & (r + 1)) - 1) {
		result = max(result, dp[j][r]);
	}
	return result;
}
void Assign(ll i, ll x, ll j, ll n, vector<vector<ll>>& dp) {
	for (; i < n; i |= i + 1) {
		dp[j][i] = max(x, dp[j][i]);
	}
}
int main() {
	ifstream inputFile("m-gaps.in", ios_base::in);
	ll n, m, sorted_el = 0, sorted_el_idx = 0, sort_idx = 0, iter_first, iter_sec, counter = 0;
	inputFile >> n >> m;
	ll upb;
	vector<ll> elements;
	vector<vector<long long>> dp;
	elements.resize(n);
	vector<ll> a(n);
	dp.resize(m + 1, vector<ll>(n,0));
	for (auto i = 0; i < n; i++) {
		inputFile >> elements[i];
		a[i] = elements[i];
	}
	sort(a.begin(), a.end());
	multimap <int, int> temp;
	for (auto &i: a)
	{
		sorted_el = i;
		sorted_el_idx = sort_idx++;
		temp.insert(pair<int, int>(sorted_el, sorted_el_idx));
	}
	for (int i = 0; i < n; ++i) {
		a[i] = temp.find(elements[i])->second;
	}
	for (auto i = 0; i < n; i++) {
		for (ll j = m; j >= 0; j--) {
			upb = max(GetMax(0, a[i]-1, j, dp), (j == 0 ? 0 : GetMax(a[i],n-1, j - 1, dp)));
			Assign(a[i], upb+1, j, n, dp);
		}
	}
	inputFile.close();
	ofstream outputFile("m-gaps.out", ios_base::out);
	outputFile << GetMax(0,n-1, m, dp);
	outputFile.close();
	return 0;
}
