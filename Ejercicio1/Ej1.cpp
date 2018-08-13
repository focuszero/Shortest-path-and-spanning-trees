#include <vector>
#include <utility>
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <climits>
#include "dsu/dsu.cpp"
#include "dsu/dsuPC.cpp"

using namespace std;

//ahora podemos usar matriz en vez de poner vector de vector
typedef vector<vector<int>> matriz;
//vector de aristas cada arista la vemos como <peso<desde,hasta>>
typedef vector<pair<int,pair<int,int> > > Aristas;


void mostrarRes(vector<string> &res){
    for (int i = 0; i < res.size(); ++i) {
        cout << res[i] << endl;
    }
}


//Calcula el peso de un agm expresado como vector de aristas
int pesoAGM(Aristas &ejes){
    int res = 0;
    for (int i = 0; i < ejes.size(); ++i) {
        res += ejes[i].first;
    }
    return res;
}


//Algoritmo de Prim O(n^2)
int Prim(matriz &grafoG){
    vector<bool> visitados = vector<bool> (grafoG.size(), false); //vector donde voy teniendo registro de qué nodos ya agregué al árbol
    vector<int> distancia = vector<int> (grafoG.size(), INT_MAX); //me guardo las distancias de los nodos al árbol y voy agregando al nodo de min distancia
    vector<int> padre = vector<int> (grafoG.size(), -1); //guardo las aristas, el padre del nodo i es el nodo p que comparte arista con i y por el cual metí a i en el árbol
    int pesoArbolito = 0;
    int nodo = 0;

    for(int i = 0; i < grafoG.size(); i++){
        if(grafoG[nodo][i] != INT_MAX/2 - 1){
            distancia[i] = grafoG[nodo][i];
            padre[i] = nodo;
        }
    }

    distancia[nodo] = 0;
    visitados[nodo] = true;

    for(int contadorVisitas = 1; contadorVisitas < grafoG.size(); contadorVisitas++){ //recorro hasta meter todos los nodos

    	int k = 0;
        while(visitados[k]){
        	k++;
        }
        int nodo_min = k;
        //tomo el nodo con mínima distancia al árbol
        for(int i = k; i < grafoG.size(); i++){
            if(not visitados[i] and distancia[i] < distancia[nodo_min]){
                nodo_min = i;
            }
        }
        visitados[nodo_min] = true; //agrego el nodo al árbol
        //actualizo distancia al árbol de los nodos no incluidos todavía que son vecino del nuevo nodo agregado
        for(int j = 0; j < grafoG.size(); j++){
            if(grafoG[nodo_min][j] != INT_MAX/2 - 1 and not visitados[j]){
                if(distancia[j] > grafoG[nodo_min][j]){
                    distancia[j] = grafoG[nodo_min][j];
                    padre[j] = nodo_min;
                }
            }
        }       
    }
    //sumo peso de aristas consideradas en el árbol
    for(int i = 0; i < grafoG.size(); i++){
            if(padre[i] != -1){
                pesoArbolito = grafoG[i][padre[i]] + pesoArbolito;
            }
        }

    return pesoArbolito;
}
//Ejercicio 1 usando Prim O(m*n*n)
vector<string> Ej1Prim(matriz &adyacencia, int n, int m, Aristas &ejes){
    matriz grafo = adyacencia;
    int pesoAgm = Prim(grafo); 
    vector<string> res(m);
     for (int i = 0; i < m; ++i) {
        grafo[ejes[i].second.first-1][ejes[i].second.second-1] = ejes[i].first - 1;
        grafo[ejes[i].second.second-1][ejes[i].second.first-1] = ejes[i].first - 1; 
        int pesoAgm1 = Prim(grafo);

        if (pesoAgm1 == pesoAgm){
            res[i] = "ninguno";
        }
        if (pesoAgm1 < pesoAgm or adyacencia[ejes[i].second.first-1][ejes[i].second.second-1] == 1){
            res[i] = "alguno";
        }

        grafo[ejes[i].second.first-1][ejes[i].second.second-1] = ejes[i].first + 1;
        grafo[ejes[i].second.second-1][ejes[i].second.first-1] = ejes[i].first + 1;
        int pesoAgm2 = Prim(grafo);

        if (pesoAgm2 > pesoAgm){
            res[i] = "todos";
        }
        grafo[ejes[i].second.first-1][ejes[i].second.second-1] = ejes[i].first;
        grafo[ejes[i].second.second-1][ejes[i].second.first-1] = ejes[i].first ;
    }

    return res;
}


//Algoritmo de Kruskal O(m*n)
Aristas kruskal(Aristas &ejes, int n, int m){
    Aristas aristas = ejes;
    Aristas solucion;
    dsu A;
    A.init(n);
    sort(aristas.begin(),aristas.end()); //O(m*logm)=O(mlogn)
    for (int i = 0; i < m; ++i) { //O(m*n)
        if ( A.find(aristas[i].second.first - 1) != A.find(aristas[i].second.second - 1) ){  //si los vertices que une la arista estan en componentes distintas
            solucion.push_back(aristas[i]);
            A.unir(aristas[i].second.first - 1 , aristas[i].second.second - 1); //uno las dos componentes
        }
    }
    return solucion;
}

//Ej1 usando Kruskal O(m*m*n)
vector<string> Ej1Kruskal(Aristas &ejes, int n, int m){
Aristas ejes1 = ejes;
vector<string> res(m);
Aristas agm = kruskal(ejes,n,m);
int pesoAgm = pesoAGM(agm);
    for (int i = 0; i < m; ++i) { //O(m*m*n)
        ejes1[i] = make_pair(ejes[i].first - 1,ejes[i].second);
        Aristas agm1 = kruskal(ejes1,n,m);
        int pesoAgm1 = pesoAGM(agm1);

        if (pesoAgm1 == pesoAgm){
            res[i] = "ninguno";
        }
        if (pesoAgm1 < pesoAgm){
            res[i] = "alguno";
        }

        ejes1[i] = make_pair(ejes[i].first + 1,ejes[i].second);
        Aristas agm2 = kruskal(ejes1,n,m);
        int pesoAgm2 = pesoAGM(agm2);

        if (pesoAgm2 > pesoAgm){
            res[i] = "todos";
        }
        ejes1[i] = make_pair(ejes[i].first,ejes[i].second);
    }

    return res;
}


//Algoritmo de Kruskal con Path Compression O(m*logn)
Aristas kruskalPC(Aristas &ejes, int n, int m){
    Aristas aristas = ejes;
    Aristas solucion;
    dsuPC A;
    A.initPC(n);
    sort(aristas.begin(),aristas.end()); //O(m*logm)=O(m*logn)
    for (int i = 0; i < m; ++i) { //O(m*alpha(1))=O(m)
        if ( A.findPC(aristas[i].second.first - 1) != A.findPC(aristas[i].second.second - 1) ){  //si los vertices que une la arista estan en componentes distintas
            solucion.push_back(aristas[i]);
            A.unirPC(aristas[i].second.first - 1 , aristas[i].second.second - 1); //uno las dos componentes
        }
    }
    return solucion;
}

//Ej1 usando Kruskal con Path Compression O(m*m*log(n))
vector<string> Ej1KruskalPC(Aristas &ejes, int n, int m){
Aristas ejes1 = ejes;
vector<string> res(m);
Aristas agm = kruskalPC(ejes,n,m);
int pesoAgm = pesoAGM(agm);
    for (int i = 0; i < m; ++i) { //O(m*m*log(n))
        ejes1[i] = make_pair(ejes[i].first - 1,ejes[i].second);
        Aristas agm1 = kruskalPC(ejes1,n,m);
        int pesoAgm1 = pesoAGM(agm1);

        if (pesoAgm1 == pesoAgm){
            res[i] = "ninguno";
        }
        if (pesoAgm1 < pesoAgm){
            res[i] = "alguno";
        }

        ejes1[i] = make_pair(ejes[i].first + 1,ejes[i].second);
        Aristas agm2 = kruskalPC(ejes1,n,m);
        int pesoAgm2 = pesoAGM(agm2);

        if (pesoAgm2 > pesoAgm){
            res[i] = "todos";
        }
        ejes1[i] = make_pair(ejes[i].first,ejes[i].second);
    }

    return res;
}



int main(int argc, char **argv){

    int n, m;

    cin >> n >> m;


    matriz matAdy (n, vector<int> (n, INT_MAX/2 - 1));  

    Aristas ejes(m);

    for (int i = 0; i < m; ++i){
        int a, b, c;
        cin >> a >> b >> c;

        ejes[i]= make_pair(c, make_pair(a,b));
        matAdy[a-1][b-1] = c;
        matAdy[b-1][a-1] = c;
    }


    if (*argv[1] == '1'){
        vector<string> resKru = Ej1Kruskal(ejes,n,m);
        mostrarRes(resKru);
    }

    if (*argv[1] == '2'){
        vector<string> resKruPC = Ej1KruskalPC(ejes,n,m);
        mostrarRes(resKruPC);
    }

    if (*argv[1] == '3'){
        vector<string> resPrim = Ej1Prim(matAdy, n, m, ejes);
        mostrarRes(resPrim);
    }



    return 0;
}

