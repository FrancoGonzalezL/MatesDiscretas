#include <bits/stdc++.h>
using namespace std;

int F(int n, int array[]){
    if (n==2){
        return max(array[0],array[1]);}
    else if (n == 3){
        return max(array[1], array[0]+array[2]);}
    else{
        return max(F(n-1, array), F(n-2, array) + array[n-1]);}
}

int main(){
    ios_base::sync_with_stdio(0); cin.tie(0);
    int n;
    cin >> n;
    int valores[n];

    for (int i = 0; i < n; i++){
        int num; cin >> num;
        valores[i]=num;}

    cout << F(n, valores) << endl;
    return 0;
}
