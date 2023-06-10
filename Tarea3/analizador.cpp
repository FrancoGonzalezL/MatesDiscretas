#include <bits/stdc++.h>
#include <fstream>
using namespace std;
//97 122 == a -> z   ascii
//32 == " "
//61 == "="
vector<vector<int>> CFG;
vector<string> Bloques;

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
bool exis_else(string name, int i, int ident_){
    ifstream ar(name);
    string line;
    int c = 0;
    while(getline(ar,line)){
        if(c<=i){c++;continue;}//avanzamos a la linea que leimos
        if(contar_ident(line)==ident_){
            if(es_if(line))return false;       
            if(es_else(line))return true;}
    }
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
    string bloque;
    int ident = 0;  
    int i = 0;
    int j = 0;

    vector<vector<int>> nodos;
    vector<int> nodo;
    stack<int> nodo_while;
    stack<tuple<int,int,bool>> nodo_if;

    while(getline(inputFile,line)){
        if(ident>contar_ident(line)){
            i++;//nuevo bloque
            while(!nodo_if.empty()){
                auto [k,ident_,e_else] = nodo_if.top();
                if(ident_<contar_ident(line)||e_else) break;
                nodos[k].push_back(i);
                nodo_if.pop();
            }
            nodo.push_back(i);
            nodos.push_back(nodo);
            nodo.clear();

            Bloques.push_back(bloque);
            bloque = "";
            }

        ident = contar_ident(line);

        if(es_if(line)){
            nodo.push_back(i+1);
            nodos.push_back(nodo);
            nodo_if.push({i,contar_ident(line),exis_else(nombre,j,contar_ident(line))});
            nodo.clear();
            i++;

            bloque+=cortar_ident(line)+"\n";
            Bloques.push_back(bloque);
            bloque = "";

        }else if(es_while(line)){
            nodo.push_back(i+1);
            nodos.push_back(nodo);
            nodo.clear();
            i++;
            nodo.push_back(i+1);
            nodos.push_back(nodo);
            nodo.clear();
            i++;

            Bloques.push_back(bloque);
            Bloques.push_back(cortar_ident(line+"\n"));
            bloque = "";

        }else if(es_else(line)){
            auto [a,b,c] = nodo_if.top();
            if(b==contar_ident(line) and c){
                nodos[a].push_back(i);
                nodo_if.pop();
            }
        }else if(es_def(line)||es_func(line)){
            bloque+=cortar_ident(line)+"\n";
        }
    j++;
    }

    Bloques.push_back(bloque);i++;
    for(int i=0;i<Bloques.size();i++){
        cout<<"bloque "<<i<<":"<<endl;
        cout<<Bloques[i]<<endl;
    }
    cout<<endl;
    for(int i=0;i<nodos.size();i++){
        cout<<i<<": ";
        for(int x: nodos[i]){
            cout<<x<<" ";
        }
        cout<<endl;
    }

    inputFile.close();//inputFile.open() si se quiere reabrir
    return 0;
}