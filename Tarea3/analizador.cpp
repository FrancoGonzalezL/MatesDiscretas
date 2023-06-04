#include <bits/stdc++.h>
#include <fstream>
using namespace std;
//97 122 == a -> z   ascii
//32 == " "
//61 == "="
vector<vector<int>> CFG;
vector<string> texto;
set<int> variables_definidas;

int buscar_variable_definida(string line){//solo una idea
    //permite saber si hay una variable sin importar identacion
    for(auto caracter: line){
        if(caracter == 32){
            continue;
        }else if (96 < caracter && caracter < 123){
            return caracter;
        }else{
            return 0;
        }
    }
    return 0;
}

int tipo_de_expresion(string line){
    for(auto caracter: line){
        if      (caracter == 32 ){continue;// == " "
        }else if(96 < caracter && caracter < 123){
            variables_definidas.insert(caracter); 
            return 1;
        }
        else if(false){}
    }
    return 0;

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
        //aqui va alguna funcion para analizar la linea

        for(int caracter: line){
            cout<<(char) caracter;
        }
        cout<<endl;
        texto.push_back(line);//solo por si es necesario, se puede guardar como un vector
        }
    inputFile.close();//inputFile.open() si se quiere reabrir

    return 0;
}