#include <bits/stdc++.h>
using namespace std;

void reflexiva(int n, vector<vector<int>>& M) {
    for (int i = 0; i < n; i++) {
        M[i][i] = 1;
    }
}

void simetrica(int n, vector<vector<int>>& M) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (M[i][j] == 1) {
                M[j][i] = 1;
            }
        }
    }
}

void transitiva(int n, vector<vector<int>>& M) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                if (M[i][k] == 1 && M[k][j] == 1) {
                    M[i][j] = 1;
                }
            }
        }
    }
}

int main() {
	ios_base::sync_with_stdio(0); cin.tie(0);
    int n;
    cin >> n;
    vector<vector<int>> M(n, vector<int>(n));
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            int num;
            cin >> num;
            M[i][j] = num;
        }
    }
    reflexiva(n,M);
    simetrica(n,M);
    transitiva(n,M);
    cout<<endl;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            cout << M[i][j] << " ";
        }
        cout << " " << endl;
    }
	return 0;
}