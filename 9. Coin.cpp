#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
using namespace std;
vector<int> z(string s) {
	int n = s.length();
	int l = 0;
	int r = 0;
	vector<int> z(n, 0);
	for (int i = 1; i < n; i++) {
		if (i <= r) {
			z[i] = min(z[i - l], r - i + 1);
		}
		while (i + z[i] < n && s[z[i]] == s[i + z[i]]) {
			z[i]++;
		}
		if (i + z[i] - 1 > r) {
			l = i;
			r = i + z[i] - 1;
		}
	}
	return z;
}
struct bigint {

    static const long long BASE = 1e9;
    static const long long SIZE = 10;

    long long digits[SIZE];

    bigint() {
        for (int i = 0; i < SIZE; i++) {
            digits[i] = 0;
        }
    }

    bigint(long long x) {
        for (int i = 0; i < SIZE; i++) {
            digits[i] = 0;
        }

        int next = 0;
        while (x) {
            digits[next++] = x % BASE;
            x /= BASE;
        }
    }

    bigint(const bigint& other) {
        for (int i = 0; i < SIZE; i++) {
            digits[i] = other.digits[i];
        }
    }

    bigint& operator=(const bigint& other) {
        for (int i = 0; i < SIZE; i++) {
            digits[i] = other.digits[i];
        }

        return *this;
    }

    void operator+=(const bigint& other) {
        for (int i = 0; i < SIZE; i++) {
            digits[i] += other.digits[i];
        }

        for (int i = 0; i < SIZE - 1; i++) {
            if (digits[i] >= BASE) {
                digits[i] -= BASE;
                digits[i + 1]++;
            }
        }
    }

    bigint operator+(const bigint& other) {
        bigint n(*this);
        n += other;
        return n;
    }

    void operator*=(const bigint& other) {
        *this = *this * other;
    }

    bigint operator*(const bigint& other) {
        bigint result;

        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE - i; j++) {
                result.digits[i + j] += digits[i] * other.digits[j];
            }
        }

        for (int i = 0; i < SIZE - 1; i++) {
            result.digits[i + 1] += result.digits[i] / BASE;
            result.digits[i] %= BASE;
        }

        return result;
    }

};

ostream& operator<<(ostream& out, const bigint& num) {
    string result;

    char buffer[10];

    for (int i = bigint::SIZE - 1; i >= 0; i--) {
        sprintf_s(buffer, "%09d", num.digits[i]);
        result += buffer;
    }

    int first_idx = result.find_first_not_of('0');
    if (first_idx == string::npos) {
        out << "0";
    }
    else {
        out << result.substr(first_idx);
    }

    return out;
}
int main() {
    ifstream inputFile("coin.in", ios_base::in);
    ofstream outputFile("coin.out", ios_base::out);
    string s;
    inputFile >> s;
	vector<int> z_mas = z(s +'$'+ s);
	int len = s.size();
	bigint res(0);
	for (auto i = len + 1; i < z_mas.size(); i++) {
        if (z_mas[i] == 2 * len - i + 1) {
            bigint temp(1);
            for (auto j = 0; j < z_mas[i]; j++) {
                temp *= 2;
            }
            res += temp;
        }
	}
    outputFile << res;
    inputFile.close();
    outputFile.close();
    return 0;
}
