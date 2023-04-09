#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
using namespace std;
typedef long long ll;

int n;
vector<vector<ll>> worker_perfomance;
vector<ll> worker_max_perfomance;
vector<ll> machine_delta;
vector<bool> is_worker_in_alt;
vector<bool> is_machine_in_alt;
vector<int> worker_to_machine;
vector<int> machine_to_worker;

bool redistibution_without_lowering(int i) {
    if (is_worker_in_alt[i])
        return false;
    is_worker_in_alt[i] = true;
    for (auto j = 0; j < n; j++) {
        if (worker_perfomance[i][j] == worker_max_perfomance[i] + machine_delta[j])
            is_machine_in_alt[j] = true;
    }
    for (auto j = 0; j < n; j++) {
        if (is_machine_in_alt[j] && 
            (worker_to_machine[j] == -1 || redistibution_without_lowering(worker_to_machine[j]))) {
            machine_to_worker[i] = j;
            worker_to_machine[j] = i;
            return true;
        }
    }
    return false;
}

int main() {
    ifstream inputFile("input.txt", ios_base::in);
    ofstream outputFile("output.txt", ios_base::out);
    inputFile >> n;
    worker_perfomance.resize(n, vector<ll>(n));
    worker_max_perfomance.resize(n);
    machine_delta.resize(n, 0);
    worker_to_machine.resize(n, -1);
    machine_to_worker.resize(n, -1);
    for (auto i = 0; i < n; i++) {
        for (auto j = 0; j < n; j++) {
            inputFile >> worker_perfomance[i][j];
            worker_max_perfomance[i] = max(worker_max_perfomance[i], worker_perfomance[i][j]);
        }
    }
    int cnt = 0;
    while (cnt != n) {
        is_worker_in_alt.resize(n, false);
        is_machine_in_alt.resize(n, false);
        bool is_changed = false;
        //пытаемся (пере)распределять станки по способностям
        for (auto i = 0; i < n; i++) {
            if (!is_worker_in_alt[i] && redistibution_without_lowering(i)) {
                cnt++;
                is_changed = true;
            }
        }
        //если по способностям не получилось, даем менее подходящие, снижая способности тех, 
        // кто не задействован, или тех, до кого можно дотянуться из незадействованных вершин по оптимальным ребрам
        if (!is_changed) {
            ll min_decrease = LLONG_MAX;
            for (auto i = 0; i < n; i++) {
                if (is_worker_in_alt[i]) {
                    //ищем минимально возможное понижение продуктивности
                    for (auto j = 0; j < n; j++) {
                        //что-то задействованное и потенциально подходящее
                        if (!is_machine_in_alt[j]) {
                            min_decrease = min(min_decrease,
                                worker_max_perfomance[i] + machine_delta[j] - worker_perfomance[i][j]);
                        }
                    }
                    for (auto j = 0; j < n; j++) {
                        if (is_worker_in_alt[i]) {
                            worker_max_perfomance[i] -= min_decrease;
                            machine_delta[is_machine_in_alt[i]] += min_decrease;
                        }
                    }
                }
            }
        }
    }
    ll result = LLONG_MAX;
    for (auto i = 0; i < n; i++) {
        result = min(result, worker_perfomance[i][machine_to_worker[i]]);
    }
    outputFile << result << '\n';
    for (auto i : machine_to_worker) {
        outputFile << i + 1 << ' ';
    }
    inputFile.close();
    outputFile.close();
    return 0;
}
