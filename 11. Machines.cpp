#include <iostream>
#include <algorithm>
#include <queue>
#include <ctime>
#include <vector>
#include <fstream>
using namespace std;
typedef long long ll;

int n;
vector<vector<ll>> worker_perfomance;
vector<ll> all_sorted_perfomances;

vector<bool> visited;
vector<bool> is_used;
bool rebuilding = false;

vector<ll> way;
vector<bool> blocked;

vector<vector<int>> graph;
vector<int> graph_matching;
int matching_amount;

vector<vector<int>> matrix;
vector<int> matching;
int cur_matching_amount;

void clear_graph() {
	matrix.assign(n, vector<int>(n, 0));
	matching.assign(n, -1);
	cur_matching_amount = 0;
}

void make_graph_main() {
	for (auto i = 0; i < n; i++) {
		for (auto j = 0; j < n; j++) {
			graph[i][j] = matrix[i][j];
		}
	}
	copy(matching.begin(), matching.end(), graph_matching.begin());
	matching_amount = cur_matching_amount;
}
void attempt_building(ll perf) {
	clear_graph();
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (worker_perfomance[i][j] >= perf) {
				if (!visited[i + n] && !visited[j]) {
					matching[i] = j;
					visited[i + n] = true;
					visited[j] = true;
					cur_matching_amount++;
				}
				else
					matrix[j][i] = 1;
			}
		}
	}
}
void change(int u, int new_v) {
	int temp = new_v;
	while (temp != u) {
		if (temp >= n) {
			matching[temp - n] = way[temp];
			matrix[way[temp]][temp - n] = 0;
		}
		else
			matrix[temp][way[temp] - n] = 1;
		temp = way[temp];
	}
	matching[u - n] = new_v;
	matrix[new_v][u - n] = 0;
	blocked[new_v] = true;
	blocked[u] = true;
}

bool redistibution(int v) {
	if (is_used[v])
		return false;
	is_used[v] = true;
	for (auto i = 0; i < n; i++) {
		auto temp = matching[i];
		if (matrix[v][i] == 1 && (matching[i] == -1 || redistibution(matching[i]))) {
			if (temp != -1) {
				matrix[temp][i] = 1;
				rebuilding = true;
			}
			matching[i] = v;
			matrix[v][i] = 0;
			return true;
		}
	}
	return false;
}

bool is_real_to_build() {
	is_used.assign(2 * n, false);
	for (auto i = 0; i < n; i++) {
		if (!visited[i]) {
			if (rebuilding)
				for (auto i = 0; i < n; i++)
					is_used[i] = false;
			rebuilding = false;
			if (!redistibution(i))
				return false;
			cur_matching_amount++;
		}
	}
	return true;
}

int get_u_v_way_dfs(int perf, int u, int v) {
	visited.assign(2 * n, false);
	visited[u] = true;
	queue<int> queue;
	queue.push(v);
	int result = -1, temp;
	while (!queue.empty()) {
		temp = queue.front();
		queue.pop();
		auto pair_v = matching[temp];
		if (!visited[pair_v] && !blocked[pair_v]) {
			way[pair_v] = temp + n;
			visited[pair_v] = true;
			if (worker_perfomance[u - n][pair_v] >= perf && (result == -1 || result > pair_v))
				result = pair_v;
			for (auto i = 0; i < n; i++) {
				if (matrix[pair_v][i] == 1 && !blocked[i + n] && !visited[i + n]) {
					visited[i + n] = true;
					way[i + n] = matching[temp];
					queue.push(i);
				}
			}
		}
	}
	return result;
}

void lexicographical(int perf) {
	blocked.assign(2 * n, false);
	way.assign(n * 2, -1);
	for (auto i = n; i < 2 * n - 1; i++) {
		if (!blocked[i]) {
			queue<int> queue;
			queue.push(i - n);
			int new_v = get_u_v_way_dfs(perf, i, i - n);
			if (new_v != -1) {
				change(i, new_v);
			}
		}
	}
}

ll dichtomy() {
	ll prev = -1;
	ll left = 0, right = n * n - 1, m;
	while (left < right) {
		m = (left + right) / 2;
		if (all_sorted_perfomances[m] < prev)
			break;
		else if (all_sorted_perfomances[m] == prev)
			left = m + 1;
		else {
			visited.assign(2 * n, false);
			attempt_building(all_sorted_perfomances[m]);
			if (is_real_to_build()) {
				prev = all_sorted_perfomances[m];
				make_graph_main();
				left = m + 1;
			}
			else
				right = m - 1;
		}
	}
	return all_sorted_perfomances[m];
}

int main() {
	//auto t = clock();
	ifstream inputFile("input.in", ios_base::in);
	ofstream outputFile("output.out", ios_base::out);
	inputFile >> n;
	worker_perfomance.resize(n, vector<ll>(n));
	all_sorted_perfomances.resize(n * n);
	graph.resize(n, vector<int>(n));
	graph_matching.resize(n);
	ll cnt = 0;
	for (auto i = 0; i < n; i++) {
		for (auto j = 0; j < n; j++) {
			inputFile >> worker_perfomance[i][j];
			all_sorted_perfomances[cnt++] = worker_perfomance[i][j];
		}
	}
	sort(all_sorted_perfomances.begin(), all_sorted_perfomances.end());
	ll result = dichtomy();
	lexicographical(result);
	outputFile << result << "\n";
	for (auto i = 0; i < n - 1; i++)
		outputFile << matching[i] + 1 << " ";
	outputFile << matching[n - 1] + 1;
	//cout << '\n' << 1.0 * (clock() - t) / CLOCKS_PER_SEC << endl;
	return 0;
}
