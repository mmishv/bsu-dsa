#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

typedef long long ll;

bool validSeq(int a, int b, int c)
{
    if (a == 1 && b == 1 && c == 1)
        return false;
    if (a == 2 && b == 2 && c == 2)
        return false;
    if (a == 0 && b == 1 && c == 2)
        return false;
    return true;
}

// количество правильных последовательностей длины length, построенной от символов _prev _last
ll calculateOnce(int length, int _prev, int _last, bool iterateOnceWithoutChecking = false)
{
    vector<vector<vector<ll>>> dp(length + 1, vector<vector<ll>>(3, vector<ll>(3, 0)));
    dp[0][_prev][_last] = 1;
    for (int i = 1; i <= length; ++i)
    {
        for (int prev = 0; prev < 3; ++prev)
        {
            for (int last = 0; last < 3; ++last)
            {
                for (int next = 0; next < 3; ++next)
                {
                    if (iterateOnceWithoutChecking || validSeq(prev, last, next))
                        dp[i][last][next] += dp[i - 1][prev][last];
                }
            }
        }
        iterateOnceWithoutChecking = false;
    }
    ll ans = 0;
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            ans += dp[length][i][j];
        }
    }
    return ans;
}

string solve(int n, ll k)
{
    int beforeLast = 0, last = 0, calculatedSymbol = 0;
    vector<int> ans;
    for (int i = 0; i < n; ++i)
    {
        calculatedSymbol = 0;
        for (int symbol = 0; symbol < 3; ++symbol)
        {
            if ((i < 2) || validSeq(beforeLast, last, symbol))
            {
                ll number = calculateOnce(n - i - 1, last, symbol, (i == 0));
                if (number <= k)
                {
                    k -= number;
                }
                else
                {
                    calculatedSymbol = symbol;
                    break;
                }
            }
        }
        ans.push_back(calculatedSymbol);
        beforeLast = last;
        last = calculatedSymbol;
    }
    string stringAnswer;
    string parts[3] = { "chu", "ka", "pi" };
    for (auto i = 0; i < ans.size(); ++i)
    {
        stringAnswer += parts[ans[i]];
    }
    return stringAnswer;
}

int main()
{
    // chu - 0
    // ka  - 1
    // pi  - 2
    int n;
    ll k;
    cin >> n >> k;
    cout << solve(n, k - 1);
    return 0;
}
