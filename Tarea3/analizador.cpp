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

bool is_alpha(int a){
    return (96<a && a<123);
}
string cortar_ident(string line){
    string s;
    auto itr = line.begin();
    while(*itr==32){
        itr++;
    }
    while(itr!=line.end()){
        s+=*itr;
        itr++;
    }
    return s;
}
int contar_ident(string line){
    int contador = 0;
    auto itr = line.begin();
    while(*itr==32){
        contador++;
        itr++;
    }
    return contador;
}
bool es_func(string line){
    //las funciones comienzan con una palabra,
    //siguen con un "(" y terminan con un ")"
    bool nombre = false;
    bool parder = false;
    bool parizq = false;
    for(int x: line){
        //ignora espacios
        if(x==32)continue;
        //ignora lo que hay en la funcion
        else if(!nombre&&is_alpha(x))nombre=true;
        else if( nombre&&!parder&&x==40)parizq=true;
        else if( nombre&& parizq&& !parder && x==41)parder=true;
        else continue;
    }
    return (nombre && parizq && parder);
}
bool es_def(string line){
    bool equal=false;
    for(int x: line){
        //si se lee "=" se esta definiendo una variable
        //si se lee "==" se esta comparando
        if(x==61)equal=!equal;
    }
    return equal;
}
bool es_if(string line){
    string s = cortar_ident(line);
    if(s.size()>2){
        // se coprueba que:
        // if ... :
        return s[0]==105&&s[1]==102&&s[s.size()-1]==58;
    }
    return false;
}
bool es_else(string line){
    string s = cortar_ident(line);
    string e = "else";
    if(s.size()<5)return false;
    for(int i=0;i<4;i++){ 
        if(s[i]!=e[i])return false;
    }
    return s[s.size()-1]==58;
}
bool es_while(string line){
    string s = cortar_ident(line);
    string w = "while";
    if(s.size()<6)return false;
    for(int i=0;i<5;i++){
        //comprobamos que tenga escrito while
        if(w[i]!=s[i])return false;
    }
    return s[s.size()-1]==58;
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

        if(ident>contar_ident(line)){
            Bloques.push_back(bloque); //cerramos bloque anterior
            bloque = "";               //reiniciamos bloque actual
            ident = contar_ident(line);//nueva identacion
        }else if(ident<contar_ident(line)){
            ident = contar_ident(line);
        }
        if(es_if(line)){
            bloque+=line;

        }else if(es_while(line)){
            cout<<" --> while"<<endl;
        }else if(es_else(line)){
            cout<<endl;
        }else if(es_def(line)||es_func(line)){
            cout<<" --> bloque"<<endl;
        }
    }

    inputFile.close();//inputFile.open() si se quiere reabrir
    return 0;
}