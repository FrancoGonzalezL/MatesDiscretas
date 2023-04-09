#include <bits/stdc++.h>
using namespace std;

vector<int> F(int n, int array[]){

    if(n==1){
        vector<int> posiciones;
        posiciones.push_back(0);
        return posiciones;}

    else if (n==2){
        vector<int> posiciones;

        if (array[0]>=array[1]){
            posiciones.push_back(0);
            return posiciones;}

        else{
            posiciones.push_back(1);
            return posiciones;}
            }

    else{
        vector<int> pos1 = F(n-1,array);
        vector<int> pos2 = F(n-2,array);
        int suma1 = 0, suma2 = 0, suma3 = array[n-1];
        for(auto itr = pos1.begin(); itr != pos1.end(); itr++){suma1+=array[*itr];}
        for(auto itr = pos2.begin(); itr != pos2.end(); itr++){suma2+=array[*itr];}
        suma2 += suma3;

        if (suma1>=suma2){

            if(suma1>=suma3){
            return pos1;}

            else{
            vector<int> pos3;
            pos3.push_back(n-1);
            return pos3;}
        }
        else{

            if(suma2>=suma3){
            pos2.push_back(n-1);
            return pos2;}

            else{
            vector<int> pos3;
            pos3.push_back(n-1);
            return pos3;}
        }
    }
}

int main(){
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