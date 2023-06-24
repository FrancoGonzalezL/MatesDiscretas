#include <bits/stdc++.h>
#include <fstream>
using namespace std;
//97 122 == a -> z   ascii
//32 == " "
//61 == "="
vector<vector<int>> CFG;

//util para separar en bloques
vector<string> Bloques;

//util para armar el CFG
vector<string> lineas; 
vector<int>    nodo_linea;

vector< pair<int,vector<string>> > Variables_Indefinidas;

bool is_alpha(int a){
    return (96<a && a<123);
}
string cortar_indent(string line){
    if(line.size()==0)return line;
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
int contar_indent(string line){
    if(line.size()==0) return 0;
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
    string s = cortar_indent(line);
    if(s.size()>2){
        // se coprueba que:
        // if ... :
        return s[0]==105&&s[1]==102&&s[s.size()-1]==58;
    }
    return false;
}
bool es_else(string line){
    string s = cortar_indent(line);
    string e = "else";
    if(s.size()<5)return false;
    for(int i=0;i<4;i++){ 
        if(s[i]!=e[i])return false;
    }
    return s[s.size()-1]==58;
}
bool es_while(string line){
    string s = cortar_indent(line);
    string w = "while";
    if(s.size()<6)return false;
    for(int i=0;i<5;i++){
        //comprobamos que tenga escrito while
        if(w[i]!=s[i])return false;
    }
    return s[s.size()-1]==58;
}
bool exis_else(int linea){
    string line;
    for(int i=linea+1;i<lineas.size()-1;i++){
        line = lineas[i];
        if(contar_indent(line)==contar_indent(lineas[linea])){
            if(es_if(line)) return false;      
            if(es_else(line))return true;
        }
    }
    return false;
}
int ult_linea(int bloque){
    //retorna index de la ultima linea contenida en el bloque
    int indx = -1;
    for(int i=0; i<lineas.size(); i++){
        if(bloque == nodo_linea[i])indx = i;
    }
    return indx;
}
bool esta_conectado(int i, int n){
    int c = 0;
    for(int j=0;j<CFG.size();j++){
        if(CFG[i][j]==1)c++;
        if(c==n)return true;
    }
    return false;
}
int encontrar_siguiente_simple(int i){
    for(int j=i+1;j<lineas.size();j++){
        string line = lineas[j];
        if(contar_indent(line)<=contar_indent(lineas[i]) && !es_else(line)){//cualquier caso posible
            return j;
        }
    }
    return lineas.size()-1;//si llegamos a "Fin"
}
int encontrar_siguiente_else(int i){
    for(int j=i+1; j<lineas.size()-1; j++){
        string line = lineas[j];
        if(contar_indent(line)==contar_indent(lineas[i]) && es_else(line)){
            return j+1;//retorna el indice del bloque siguiente al else
        }
    }
    return -1;
}
int ultimo_con_igual_indent(int i){
    int k = i;
    for(int j=i;j<lineas.size()-1;j++){
        string line = lineas[j];
        if(contar_indent(line)==contar_indent(lineas[i]) && !es_else(line)) k = j;
        if(contar_indent(line)<contar_indent(lineas[i]))break;
    }
    return k;
}
void conectar_hijos(int a, int b, int c){
    // a = linea con primer hijo if
    // b = linea con primer hijo else
    // c = linea con la que quiero conectar
    // para el hijo del if buscamos el ultimo y lo conectamos a c
    int k = ultimo_con_igual_indent(a);
    if(es_def(lineas[k])||es_func(lineas[k])){
        CFG[nodo_linea[k]][nodo_linea[c]] = 1;
    }else if(es_if(lineas[k])){
        CFG[nodo_linea[k]][nodo_linea[k]+1] = 1;//se conecta con el hijo del if
        if(!exis_else(k)){
            CFG[nodo_linea[k]][nodo_linea[c]] = 1;//conectar nodo con el nodo deseado
            conectar_hijos(ult_linea(nodo_linea[k]+1),-1,c);
        }else{
            CFG[nodo_linea[k]][nodo_linea[encontrar_siguiente_else(k)]] = 1;//conectar con el hijo de else
            conectar_hijos(ult_linea(nodo_linea[k]+1),encontrar_siguiente_else(k),c);
        }
    }else if(es_while(lineas[k])){
        CFG[nodo_linea[k]][nodo_linea[c]] = 1;
        CFG[nodo_linea[k]][nodo_linea[k]+1] = 1;
        conectar_hijos(ult_linea(nodo_linea[k]+1),-1,k);
    }

    //lo mismo para el hijo del else
    if (b == -1) return;
    k = ultimo_con_igual_indent(b);
    if(es_def(lineas[k])||es_func(lineas[k])){
        CFG[nodo_linea[k]][nodo_linea[c]] = 1;
    }else if(es_if(lineas[k])){
        CFG[nodo_linea[k]][nodo_linea[k]+1] = 1;//se conecta con el hijo del if
        if(!exis_else(k)){
            CFG[nodo_linea[k]][nodo_linea[c]] = 1;//conectar nodo con el nodo deseado
            conectar_hijos(ult_linea(nodo_linea[k]+1),-1,c);
        }else{
            CFG[nodo_linea[k]][nodo_linea[encontrar_siguiente_else(k)]] = 1;//conectar con el hijo de else
            conectar_hijos(ult_linea(nodo_linea[k]+1),encontrar_siguiente_else(k),c);
        }
    }else if(es_while(lineas[k])){
        CFG[nodo_linea[k]][nodo_linea[c]] = 1;
        CFG[nodo_linea[k]][nodo_linea[k]+1] = 1;
        conectar_hijos(ult_linea(nodo_linea[k]+1),-1,k);
    }
}
//---------------------------------------------------------------------
string ifwhile_true(string line, bool t_f=false, bool true_=false){//funcion solo para dejar string en el formato pedido
    string nueva_linea;
    if(es_while(line)){
        for(int i=6;i<line.size()-1;i++){
            nueva_linea += line[i];
        }         
    }else if(es_if(line)){
        for(int i=3;i<line.size()-1;i++){
            nueva_linea += line[i];
        }
    }
    if(t_f){
        if(true_){nueva_linea += " == True";}
        else     {nueva_linea += " == False";}
    }
    return nueva_linea;
}
int variables_def(string line){
    for(int x:line){
        if(isalpha(x)){return x;}//la primera letra que leemos: x =...
    }
    return 0;
}
vector<int> variable_utl(string line){
    vector<int> variables;
    auto itr = line.begin();
    if(es_if(line)||es_while(line)){
        line = ifwhile_true(line);
        itr = line.begin();
        //leemos todas las variables de la condicion
        if(es_func(line))return variable_utl(line);
        while(itr!=line.end()){
            if(is_alpha(*itr)) variables.push_back(*itr);
            itr++;
        }
    }else if(es_func(line)){//se define una variable con una funcion
        while(*itr!=40)itr++;
        bool str =false;
        while(itr!=line.end()){
            if(*itr==34||*itr==39)str=!str;
            if(is_alpha(*itr)&&!str) variables.push_back(*itr);
            itr++;
        }

    }else if(es_def(line)){//definicion normal
        while(*itr!=61)itr++;//avanzar hasta llegar al igual
        while(itr!=line.end()){
            if(is_alpha(*itr)) variables.push_back(*itr);
            itr++;
        }
    }
    return variables;
}
void variable_ind_encontrada(char a, vector<string> camino){
    cout<<endl<<"Variable: "<< a <<endl<<"Camino: ";
    for(int i=0;i<camino.size();i++){
        cout<< camino[i];
        if(i<camino.size()-1)cout<<", ";
    }
}
void variables_indefinidas(int bloque_, vector<int> nodos_visitados, set<int> variables_, vector<string> camino){
    if(bloque_==CFG.size()-1) return; //linea == "Fin"
    string line;
    vector<int> var_;
    int var;
    for(int i=0; i<nodo_linea.size(); i++){
        if(nodo_linea[i]==bloque_){//pertenece al bloque 
            line = cortar_indent(lineas[i]);
            camino.push_back(line);//se agrega al camino nodo que estamos leyendo
            var_ = variable_utl(line);//variables utilizadas
            for(int a: var_){
                if(variables_.find(a)==variables_.end()){//si alguan variable no fue definida antes
                    string c = camino[camino.size()-1];
                    if(es_if(camino[camino.size()-1])||es_while(camino[camino.size()-1])){
                        camino[camino.size()-1] = ifwhile_true(camino[camino.size()-1]);
                    }
                    Variables_Indefinidas.push_back( {a,camino} );
                    //variable_ind_encontrada((char) a, camino);//
                    camino[camino.size()-1] = c;
                }
            }
            if(es_def(line)){//si es que se define una variable
                var = variables_def(line);
                variables_.insert(var);//agregamos la variable que definimos
            }
        }
    }
    //seguimos por los posibles caminos
    for(int j=0; j<CFG.size(); j++){
        if(CFG[bloque_][j] && !nodos_visitados[j]){
            nodos_visitados[j] = 1;
            bool t_f = !(j - bloque_ - 1); 
            string c = camino[camino.size()-1];
            if(es_if(camino[camino.size()-1])||es_while(camino[camino.size()-1])){
                //si es una condicion y no sigue su camino directo 
                camino[camino.size()-1] = ifwhile_true(camino[camino.size()-1], true, t_f);
            }
            variables_indefinidas(j, nodos_visitados, variables_, camino);
            camino[camino.size()-1] = c;
        }
    }
}
//---------------------------------------------------------------------
int main(){
    string nombre; cin>>nombre;
    ifstream inputFile(nombre);
    if(!inputFile){
        cout<<"ERROR: archivo no encontrado";
        return 1;}
    string line;
    string bloque;
    int indent = 0;  
    int i = 0;
    while(getline(inputFile,line)){
        //para terminar bloques si se reduce indentacion
        if(contar_indent(line)<indent && !es_else(line) && bloque.size()>0){
            i++;
            Bloques.push_back(bloque);
            bloque = "";
        }
        indent = contar_indent(line);//se actualiza indentacion

        if(es_if(line)){
            lineas.push_back(line);
            nodo_linea.push_back(i);
            i++;

            bloque+=cortar_indent(line)+"\n";
            Bloques.push_back(bloque);
            bloque = "";
        }else if(es_while(line)){
            lineas.push_back(line);
            if(bloque.size()>0){
                Bloques.push_back(bloque);
                i++;}
            nodo_linea.push_back(i);
            i++;
            Bloques.push_back(cortar_indent(line+"\n"));
            bloque = "";
        }else if(es_else(line)){
            nodo_linea.push_back(-1);
            i++;
            lineas.push_back(line);
            Bloques.push_back(bloque);
            bloque = "";
        }else if(es_def(line)||es_func(line)){
            nodo_linea.push_back(i);
            lineas.push_back(line);
            bloque+=cortar_indent(line)+"\n";
        }
    }
    if(bloque.size()>0){
        Bloques.push_back(bloque);
        i++;
    }
    lineas.push_back("Fin");
    Bloques.push_back("Fin");
    nodo_linea.push_back(i);


    CFG.assign(Bloques.size(),vector<int>(Bloques.size(),0));//Matriz de adyacencia
    for(int i=0;i<Bloques.size()-1;i++){//menos Fin (no conecta con nada)
        //buscaremos que tipo de bloque es, con que se conecta, etc..
        //primero buscamos el indice del bloque
        //buscamos el indice en los nodos
        //buscamos las lineas contenidas
        //revisamos la info de las lineas

        //conviene tener una funcion que encuentre el siguiente
        // puede ser que con el indice i buscamos la ultima linea con el indice i
        // identificamos el bloque actual
        // identificamos si pertenece a algun while
        // caso contrario el siguiente debe ser siguiente bloque

        //los bloques que terminan en instrucciones simples o definiciones siempre conectan solo con 1


        int j = ult_linea(i);
        string linea = lineas[j]; 
        if (es_while(linea) && !esta_conectado(i,2)){
            CFG[i][i+1] = 1;
        //si leemos un while
        // --> conectamos el while con el siguiente y con el siguiente fuera del while (funcion encontrar siguiente no contenido y siguiente simple)
            int nodo_ = encontrar_siguiente_simple(j);
            CFG[i][nodo_linea[nodo_]] = 1;
        // --> conectamos el ultimo bloque contenido en el while con el mismo
        //     el ultimo debe tener igual indentacion que el primer bloque despues del while
        //     si es una instruccion o un while la conectamos simplemente
        //     si es un if, conectamos sus nodos finales con el while 
        //          --> si es un if simple conectar la ultima instruccion del if con el while
        //          --> si es un if_else conectar las 2 ultimas instrucciones con el while
            conectar_hijos(ult_linea(i+1),-1,j);
        }else if(es_if(linea) && !esta_conectado(i,2) ){ 
        //si el bloque termina con una condicion if
        // sin else:
        // --> conectar el bloque del if con el siguiente y con el siguiente que tenga igual o menor indentacion
        // con else:
        // --> conectar el bloque con el siguiente y el siguiente despues del else
        // --> la ultima instruccion contenida en el if debe estar conectada con lo mismo que se conecta la instruccion final del else
            CFG[i][i+1] = 1;
            if(!exis_else(j)){//si no existe un else para el if
                int nodo_ = encontrar_siguiente_simple(j);
                CFG[i][nodo_linea[nodo_]] = 1;
                conectar_hijos(ult_linea(i+1),-1, nodo_);
            }else{
                int nodo_ = encontrar_siguiente_else(j);
                CFG[i][nodo_linea[nodo_]] = 1;
                conectar_hijos(ult_linea(i+1),ult_linea(nodo_linea[nodo_]),encontrar_siguiente_simple(j));
            }

        }else if((es_def(linea)||es_func(linea)) && !esta_conectado(i,1)){
        // si es una instruccion simple
        // --> conectar con el siguiente (a no ser que ya este conectado)
        //si el bloque actual termina con igual indentacion con la que comienza el bloque siguiente entonces conectar directo
        // --> debe se alguna condicion if o while
            CFG[i][i+1] = 1;
        }
    }



    if(false){
    for(int i=0;i<Bloques.size();i++){
        cout<<"bloque "<<i+1<<":"<<endl;
        cout<<Bloques[i]<<endl;
    }
    cout<<endl<<"Matriz de adyacencia:"<<endl;
    cout<<"   ";
    for(int i=0;i<CFG.size();i++){
        cout<<i+1<<" ";
        if(i<9){cout<<" ";}
    }
    cout<<endl;

    for(int i=0;i<CFG.size();i++){
        cout<<i+1<<" ";
        if(i<9){cout<<" ";}

        for(int j=0;j<CFG.size();j++){
            cout<<CFG[i][j]<<"  ";
        }
        cout<<endl;
    }
    }
    cout<<"CFG"<<endl;
    cout<<"Nodos: "<<CFG.size()<<endl;
    int arcos = 0;for(auto nodo:CFG){for(int arco:nodo)arcos+=arco;}
    cout<<"Arcos: "<<arcos<<endl;
    cout<<"Componentes conexos: " << "1"<<endl<<endl;

    vector<int> a_o(CFG.size(),0);
    set<int> v_d;
    vector<string> c;
    cout<<"Variables Indefinidas";
    variables_indefinidas(0, a_o, v_d, c);
    sort(Variables_Indefinidas.begin(),Variables_Indefinidas.end());

    for(auto [var,camino]: Variables_Indefinidas){
        variable_ind_encontrada((char) var, camino);
    }
    cout<<endl<<endl;

    cout<<"Complejidad ciclomatica"<<endl;
    cout<<arcos - CFG.size() + 2<<endl;

    if(false){//permite visualizar Bloques y CFG
        cout<<endl<<"Bloques:"<<endl<<endl;
        for(int i=0;i<Bloques.size();i++){
            cout<<"Bloque "<<i+1<<":"<<endl;
            cout<< Bloques[i] <<endl;
        }
        cout<<endl;
        cout<<endl<<"Matriz de adyacencia:"<<endl;
        cout<<"   ";
        for(int i=0;i<CFG.size();i++){
            cout<<i+1<<" ";
            if(i<9){cout<<" ";}
        }
        cout<<endl;

        for(int i=0;i<CFG.size();i++){
            cout<<i+1<<" ";
            if(i<9){cout<<" ";}

            for(int j=0;j<CFG.size();j++){
                cout<<CFG[i][j]<<"  ";
            }
            cout<<endl;
        }
    }
    inputFile.close();//inputFile.open() si se quiere reabrir
    return 0;
}