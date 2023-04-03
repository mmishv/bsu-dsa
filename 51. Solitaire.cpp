#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

//put i-th card on j-th, i <= j
void optimal(int i, int j, const int* cards, long long** dp) {
    if (i == j) {
        dp[i][j] = 0;
        return;
    }
    long long t;
    for (int k = i; k < j; k++) {
        t = dp[i][k] + dp[k + 1][j] + abs(cards[k] - cards[j]);
        if (dp[i][j] == 0 || dp[i][j] > t)
            dp[i][j] = t;
    }     
}

int main() {
    ifstream inputFile("input.txt");
    int n;
    inputFile >> n;
    int num;
    int c = 0;

    auto cards = new int[n];
    while (inputFile >> num)
        cards[num - 1] = c++;
    inputFile.close();

    auto dp = new long long* [n];
    for (int i = 0; i < n; i++) {
        dp[i] = new long long[n](0);
    }
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n - i; j++)
            optimal(j, i + j, cards, dp);

    ofstream outputFile("output.txt");
    outputFile << dp[0][n-1];
    outputFile.close();

    for (int i = 0; i < n; i++) {
        delete[] dp[i];
    }
    delete[] cards;

    return 0;
}
