#include <bits/stdc++.h>
#include <fstream>
using namespace std;

vector<vector<int>> CFG;
vector<string> texto;

int main(){
    //abrir el archivo
    string nombre; cin>>nombre;
    ifstream inputFile(nombre);
    if(!inputFile){
        cout<<"ERROR: archivo no encontrado";
        return 1;}
    //contar lineas
    string line;
    int n=0;//necesario para saber tamaño del grafo
    while(getline(inputFile,line)){
        n++;}

    CFG.assign(n,vector<int>(n,0));//grafo con solo 0's
    //abrir y cerrar (necesario para volver a leer)
    inputFile.close(); 
    inputFile.open(nombre);
    //cout a las lineas
    line = "";
    while(getline(inputFile,line)){
        texto.push_back(line);//solo por si es necesario, se puede guardar como un vector
                              //para tener las lineas indexadas
        cout<<line<<endl;}
    cout<<n<<endl;
    inputFile.close();

    //codigo para procesar aqui


    return 0;
}