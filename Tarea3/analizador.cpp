#include <bits/stdc++.h>
#include <fstream>
using namespace std;
//97 122 == a -> z   ascii
//32 == " "
//61 == "="
vector<vector<int>> CFG;
vector<string> Bloques;
vector<string> texto;
set<int> variables_definidas;

int contar_ident(string line){
    int contador = 0;
    auto itr = line.begin();
    while(*itr==32){
        contador++;
        itr++;
    }
    return contador;
}

bool es_funcion(string line, int i){
    //de momento que funcione solo con print() o con f(x,...,z)
    return false;
}
bool es_def(string line, int i){

    return false;
}

bool es_instruccion(string line){
    //puede ser una funcion simplemente
    //o una definicion de variable
    for(int i=0;i<line.size();i++){
        if(line[i]==32)continue;

        if (es_funcion(line, i)){ 
            return true;
        }else if(es_def(line, i)){
            return true; 
        }else{return false;}
    } 
}

bool es_if(string line){
    return false;
}
bool es_while(string line){
    return false;
}



int main(){
    //abrir el archivo
    string nombre; cin>>nombre;
    ifstream inputFile(nombre);
    if(!inputFile){
        cout<<"ERROR: archivo no encontrado";
        return 1;}

    string line;
    while(getline(inputFile,line)){
        int ident = 0;  
        string bloque;

        if(ident!=contar_ident(line)){
            Bloques.push_back(bloque); //cerramos bloque anterior
            bloque = "";               //reiniciamos bloque actual
            ident = contar_ident(line);//nueva identacion
        }

        if(es_instruccion(line)){
            continue;
        }else if(es_if(line)){
            continue;
        }else if(es_while(line)){
            continue;
        }

        }
    inputFile.close();//inputFile.open() si se quiere reabrir

    return 0;
}