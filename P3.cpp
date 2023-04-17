#include <bits/stdc++.h>
using namespace std;

vector<int> F(int n, int lista[], int &Sum){

    if(n==0){
        vector<int> posiciones;
        Sum = 0;
        return posiciones;}

    else if (n==1){
        vector<int> posiciones;
        Sum = max(lista[0],0);
        if (Sum==lista[0]){posiciones.push_back(0);}
        return posiciones;}

    else{
        vector<int> Fn_1 = F(n-1,lista,Sum);
        int sumFn_1 = Sum;
        vector<int> Fn_2 = F(n-2,lista,Sum);
        int sumFn_2 = Sum + lista[n-1];

        Sum = max(sumFn_1,sumFn_2);

        if(Sum == sumFn_1){
            return Fn_1;
        }else{
            Fn_2.push_back(n-1);
            return Fn_2;}
        }
}

int main(){
    int n;cin >> n;int Lista[n];
    for (int i = 0; i < n; i++){int num; cin >> num;Lista[i]=num;}

    int suma;
    vector<int> MaximaSat = F(n, Lista, suma);

    if(MaximaSat.size()>0){
    for(int posicion: MaximaSat){
        cout<< posicion + 1 <<" ";}
    }else{cout<<0;}
    cout<<endl<<suma<<endl;

    return 0;
}