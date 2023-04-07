#include <bits/stdc++.h>
using namespace std;

vector<int> F(int n, int array[]){
    if (n==2){
        vector<int> posiciones;
        if (array[0]>=array[1]){posiciones.push_back(0);}
        else{posiciones.push_back(1);}

        return posiciones;}
    else if (n == 3){
        vector<int> posiciones;
        if (array[0]+array[2]>=array[1]){
            posiciones.push_back(0);posiciones.push_back(2);
            return posiciones;}
        else{
            posiciones.push_back(1);
            return posiciones;}
        }
    else{
        vector<int> pos1 = F(n-1,array);
        vector<int> pos2 = F(n-2,array);
        int suma1 = 0;
        int suma2 = 0;
        for(auto itr = pos1.begin(); itr != pos1.end(); itr++){suma1+=*itr;}
        for(auto itr = pos2.begin(); itr != pos2.end(); itr++){suma2+=*itr;}
        if(suma2+array[n-1]>=suma1){
            pos2.push_back(n-1);
            return pos2;}
        else{return pos1;}
        }
}

int main(){
    ios_base::sync_with_stdio(0); cin.tie(0);
    int n;cin >> n;int valores[n];
    for (int i = 0; i < n; i++){int num; cin >> num;valores[i]=num;}

    vector<int> result = F(n, valores);

    int suma=0;
    for(auto itr = result.begin(); itr != result.end(); itr++){
        cout<< *itr+1 <<" ";
        suma+= valores[*itr];
    }
    cout<<endl<< suma <<endl;
    return 0;
}
