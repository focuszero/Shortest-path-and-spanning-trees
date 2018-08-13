#include <vector>
#include <utility>
#include <iostream>
#include <algorithm>
#include <climits>
#include <set>
#include <queue>
#include <chrono>
#include <stdlib.h>
#include <math.h>
#include <map>

using namespace std;

//ahora podemos usar matriz en vez de poner vector de vector
typedef vector<vector<int>> matriz;
//vector de aristas cada arista la vemos como <peso<desde,hasta>>
typedef vector<pair<int,pair<int,int> > > Aristas;
//listas de adyacencia
//en el lugar iesimo del vector de más afuera están las aristas del nodo i+1
//guardadas como vector<pair<int,int>>,
//primera coordenada es el vértice con el que tiene la arista y segunda coordenada es el peso
typedef vector<vector<pair<int,int> > > listaAdy;
//Para la salida ej2
//<<desde,hasta>,costo>
typedef map<pair<int,int>,int> resultadoOrdenado;          //Resultados en un Map

//infinito ahora lo representamos con inf
int inf = INT_MAX/2 - 1;

//toma dos enteros cualesquiera entre 1 y n distintos
pair<int,int> parDeNodos(int n){
	int nodos [n];
	for(int i = 0; i < n; i++){
		nodos[i] = i + 1;
	}
	random_shuffle(&nodos[0], &nodos[n-1]);
	return make_pair(nodos[0], nodos[1]);
}

//Muestra matrices por pantalla
void mostrarMatriz(matriz &mat, string nombre) {
    cout << nombre << endl;
    for (int j = 0; j < mat.size(); ++j) {
        for (int k = 0; k < mat.size(); ++k) {
        	if(mat[j][k] == inf){
        		cout << "INF" << " ";
        	} else {
            	cout << mat[j][k] << " ";
        	}
        }
        cout << endl;
    }
}

//Mostrar map
void mostrarMap(map<pair<int,int>,int> &map1, string nombre){
    // cout << nombre << endl;
    // cout << "Desde Hasta Costo" << endl;
    for (auto it = map1.begin(); it != map1.end(); ++it){
        cout << it->first.first + 1 << " " << it->first.second + 1<< " " << it->second << endl;
    }
}

pair<int,int> nodoYnafta(int i){
pair<int, int> res = make_pair(div(i,61).quot, i%61);
    return res;
}

int estimadora(int i, int j, int n){
  div_t coordenadas_i = div(i,ceil(sqrt(n)));
  div_t coordenadas_j = div(j,ceil(sqrt(n)));		
  int res = sqrt(pow(coordenadas_i.quot-coordenadas_j.quot,2) + pow(coordenadas_i.rem-coordenadas_j.rem,2));
  return res;
}



//Algoritmo de Dijkstra O(n^2)
vector<int> Dijkstra(listaAdy &pesos, int nodo){
	nodo = nodo - 1;
	int n = pesos.size();
	int dist_infinito = INT_MAX/2 - 1; 
	vector<int> distancias_nodo = vector<int> (n, dist_infinito);           // Vector PI
	distancias_nodo[nodo] = 0;                                             // Distancia de un nodo a si mismo
	vector<pair<bool,int> > noVisitados = vector<pair<bool,int> > (n, make_pair(true, dist_infinito));  // Vector (no visite al nodo?, distancia)
	noVisitados[nodo] = make_pair(true,0);
	
	int contadorVisitados = 0;
	while(contadorVisitados != n){                                      //O(n)
		int k = 0;
		while(not noVisitados[k].first){                                //O(n)
			k++;
		}
		int nodoMin = k;
		while(k < n){                                                   //O(n)
			if(noVisitados[k].first and noVisitados[k].second < noVisitados[nodoMin].second){
				nodoMin = k;
			}
			k++;
		}
		noVisitados[nodoMin] = make_pair(false,0);
		for(int i = 0; i < pesos[nodoMin].size(); i++){                 //O(n)
			if(noVisitados[pesos[nodoMin][i].first].first){
				if(distancias_nodo[pesos[nodoMin][i].first] > distancias_nodo[nodoMin] + pesos[nodoMin][i].second){
					distancias_nodo[pesos[nodoMin][i].first] = distancias_nodo[nodoMin] + pesos[nodoMin][i].second;
					noVisitados[pesos[nodoMin][i].first] = make_pair(true,distancias_nodo[pesos[nodoMin][i].first]);
				}

			}
		}
		contadorVisitados++;
	}
	return distancias_nodo;
}


//Algoritmo de Dijkstra con priority queue O((n+m)log(n))

vector<int> DijkstraPQ(listaAdy &pesos, int nodo){
	nodo = nodo - 1;
	int n = pesos.size();
	int dist_infinito = INT_MAX/2 - 1;
	vector<int> distancias_nodo = vector<int> (n, dist_infinito);   //resultado final distancias al nodo
	distancias_nodo[nodo] = 0;
	set<pair<int,int>> noVisitados;                                 //distancias temporales al nodo

	for(int i = 0; i < n; i++){                                     //inicializo distancias temporales en infinito
		pair<int,int> nodoYdistancia = make_pair(dist_infinito,i);
		noVisitados.insert(nodoYdistancia);
	}

	noVisitados.erase(make_pair(nodo,dist_infinito));
	noVisitados.insert(make_pair(0,nodo));

	while(noVisitados.size() != 0){
		auto it_nodo_min = noVisitados.begin();                     //busco el vertice con minima distancia al nodo
		    int nodo_min = (*it_nodo_min).second;
			noVisitados.erase(it_nodo_min);                         //lo saco de las distancias todavia no calculadas

			for(int i = 0; i < pesos[nodo_min].size(); i++){        //actualizo distancias de vecinos de ser necesario
				if(noVisitados.find(make_pair(distancias_nodo[pesos[nodo_min][i].first],pesos[nodo_min][i].first)) != noVisitados.end()){
					pair<int,int> vecino = make_pair(distancias_nodo[pesos[nodo_min][i].first],pesos[nodo_min][i].first);

					if(distancias_nodo[pesos[nodo_min][i].first] > distancias_nodo[nodo_min] + pesos[nodo_min][i].second){
						distancias_nodo[pesos[nodo_min][i].first] = distancias_nodo[nodo_min] + pesos[nodo_min][i].second;
						pair<int,int> vecinoActualizado = make_pair(distancias_nodo[pesos[nodo_min][i].first], pesos[nodo_min][i].first);
						noVisitados.erase(vecino);
						noVisitados.insert(vecinoActualizado);
					}	
				}
			}
		}
	return distancias_nodo;
}

//Dijkstra uno contra uno
int DijkstraPQunoAuno(listaAdy &pesos, int nodo_src, int nodo_dst){
	nodo_src = nodo_src - 1;
	nodo_dst = nodo_dst - 1;
	int n = pesos.size();
	int dist_infinito = INT_MAX/2 - 1;
	vector<int> distancias_nodo = vector<int> (n, dist_infinito);   //resultado final distancias al nodo
	distancias_nodo[nodo_src] = 0;
	set<pair<int,int>> noVisitados;                                 //distancias temporales al nodo

	for(int i = 0; i < n; i++){                                     //inicializo distancias temporales en infinito
		pair<int,int> nodoYdistancia = make_pair(dist_infinito,i);
		noVisitados.insert(nodoYdistancia);
	}

	noVisitados.erase(make_pair(nodo_src,dist_infinito));
	noVisitados.insert(make_pair(0,nodo_src));
	bool loEncontre = false;
	while(not loEncontre){
		auto it_nodo_min = noVisitados.begin();                     //busco el vertice con minima distancia al nodo
		    int nodo_min = (*it_nodo_min).second;
			noVisitados.erase(it_nodo_min);                         //lo saco de las distancias todavia no calculadas

			for(int i = 0; i < pesos[nodo_min].size(); i++){        //actualizo distancias de vecinos de ser necesario
				if(noVisitados.find(make_pair(distancias_nodo[pesos[nodo_min][i].first],pesos[nodo_min][i].first)) != noVisitados.end()){
					pair<int,int> vecino = make_pair(distancias_nodo[pesos[nodo_min][i].first],pesos[nodo_min][i].first);

					if(distancias_nodo[pesos[nodo_min][i].first] > distancias_nodo[nodo_min] + pesos[nodo_min][i].second){
						distancias_nodo[pesos[nodo_min][i].first] = distancias_nodo[nodo_min] + pesos[nodo_min][i].second;
						pair<int,int> vecinoActualizado = make_pair(distancias_nodo[pesos[nodo_min][i].first], pesos[nodo_min][i].first);
						noVisitados.erase(vecino);
						noVisitados.insert(vecinoActualizado);
					}	
				}
			}
			
			if(nodo_min == nodo_dst){
				loEncontre = true;
			}
		}
	return distancias_nodo[nodo_dst];
}
//Algoritmo A* (Solo para grafos que cumplan la desigualdad triangular)

int A_estrella(listaAdy &pesos, int nodo_src, int nodo_dst){
	nodo_src = nodo_src - 1;
	nodo_dst = nodo_dst - 1;
	int n = pesos.size();
	int dist_infinito = INT_MAX/2 - 1; 
	vector<int> distancias_nodo = vector<int> (n, dist_infinito);           // Vector PI
    distancias_nodo[nodo_src] = 0;                                          // Distancia de un nodo a si mismo
	vector<pair<bool,int> > noVisitados = vector<pair<bool,int> > (n, make_pair(true, dist_infinito));  // Vector (no visite al nodo?, distancia)
	noVisitados[nodo_src] = make_pair(true,0);
	
	bool loEncontre = false;
	while(not loEncontre){                                                  //O(n)
		int k = 0;
		while(not noVisitados[k].first){                                    //O(n)
			k++;
		}
		int nodoMin = k;
		while(k < n){                                                       //O(n)
			if(noVisitados[k].first and noVisitados[k].second + estimadora(k + 1, nodo_dst + 1, n) < noVisitados[nodoMin].second + estimadora(nodoMin + 1, nodo_dst + 1, n)){ //busco el mas proximo con la funcion f(n) = g(n) + h(n)
				nodoMin = k;																												// donde g es la distancia original y h es "distancia euclidea"
			}
			k++;
		}
		noVisitados[nodoMin] = make_pair(false,0);
		for(int i = 0; i < pesos[nodoMin].size(); i++){                     //O(n)
			if(noVisitados[pesos[nodoMin][i].first].first){
				if(distancias_nodo[pesos[nodoMin][i].first] > distancias_nodo[nodoMin] + pesos[nodoMin][i].second){
					distancias_nodo[pesos[nodoMin][i].first] = distancias_nodo[nodoMin] + pesos[nodoMin][i].second;
					noVisitados[pesos[nodoMin][i].first] = make_pair(true,distancias_nodo[pesos[nodoMin][i].first]);
				}

			}
		}
		if(nodoMin == nodo_dst){
			loEncontre = true;
		}
	}
	return distancias_nodo[nodo_dst];
}



//Algoritmo de Bellman-Ford
vector<int> BellmanFord(listaAdy &ejes, int desde, int n){
    vector<int> phi(n, inf); 								//phi es el vector que me va a ir diciendo los caminos minimos entre desde y los indices del vector
    phi[desde-1] = 0;
    for (int i = 0; i < n; ++i) {							//voy a recorrer el vector phi
        for (int k = 0; k < ejes.size(); ++k) {             //Aca voy a relajar aristas si es posible
            for (int j = 0; j < ejes[k].size(); ++j) {
                if (phi[ejes[k][j].first] > phi[k] + ejes[k][j].second) {
                    phi[ejes[k][j].first] = phi[k] + ejes[k][j].second;
                }
            }
        }
    }
    return phi;
}


//Algoritmo de Floyd-Warshall
matriz Floyd_Warshall(matriz &caminos){
    matriz res = caminos;
    for(int k = 0; k < res.size(); ++k){
        for(int i = 0; i < res.size(); ++i){
            for(int j = 0; j < res.size(); ++j){
                int dt = res[i][k] + res[k][j];
                if(res[i][j] > dt){							// Si el camino es mejor que el anterior, lo reemplazo
                    res[i][j] = dt;
                }
            }
        }
    }
    return res;
}


//Algoritmo de Dantzig
matriz dantzig(matriz &caminos){
    matriz res = caminos;
    for(int k = 0; k < caminos.size()-1; ++k){
        for(int i = 0; i <= k; ++i){							// Calculo el camino minimo al nuevo nodo

            int min = res[i][0] + res[0][k+1];
            int min2 = res[k+1][0] + res[0][i];

            for(int j = 0; j <= k; ++j){                        // Calculo los nuevos mínimos
                int actual = res[i][j] + res[j][k+1];
                if(actual <= min){
                    min = actual;
                }

                int actual2 = res[k+1][j] + res[j][i];
                if(actual2 <= min2){
                    min2 = actual2;
                }
            }

            res[i][k+1] = min;
            res[k+1][i] = min2;

        }																

        for(int i = 0; i <= k; ++i){							// Recalculo la matriz interna con el nuevo nodo
            for(int j = 0; j <= k; ++j){
                if (res[i][j] > res[i][k+1] + res[k+1][j]) {
                    res[i][j] = res[i][k+1] + res[k+1][j];
                }
            }
        }
    }
    return res;
}



bool puedoIr(matriz &matAdy, vector<int> &precioNafta, int i, int j){
bool res = false;
bool hayCamino = false;
bool meAlcanzaLaNafta = false;
    if (matAdy[nodoYnafta(i).first][nodoYnafta(j).first] != inf){
        hayCamino = true;
    }
bool siLLego = nodoYnafta(i).second >= matAdy[nodoYnafta(i).first][nodoYnafta(j).first];
bool igualNafta = nodoYnafta(i).second - matAdy[nodoYnafta(i).first][nodoYnafta(j).first] == nodoYnafta(j).second;
    if (siLLego && igualNafta){
        meAlcanzaLaNafta = true;
    }
    if (hayCamino && meAlcanzaLaNafta){
        res = true;
    }
    return res;
}

matriz generarMatriz(matriz &matAdy, int n, vector<int> &precioNafta){
    matriz res(61*n, vector<int> (61*n, inf));
    for (int i = 0; i < 61*n; ++i) {                            //lleno la matriz a la cual le voy a aplicar camino minimo
        for (int j = 0; j < 61*n; ++j) {
        		if (i == j) {
        			res[i][j] = 0;
        		}
                if (nodoYnafta(i).first == nodoYnafta(j).first && i < j && nodoYnafta(i).second + 1 == nodoYnafta(j).second) {//cargo nafta
                    res[i][j] = precioNafta[nodoYnafta(i).first] * nodoYnafta(j).second - precioNafta[nodoYnafta(i).first]*nodoYnafta(i).second;
                }
                if (nodoYnafta(i).first != nodoYnafta(j).first && puedoIr(matAdy,precioNafta,i,j)){//me muevo a otra ciudad
                    res[i][j] = 0;
                }
        }
    }
    return res;
}

listaAdy generarLista(matriz &matAdy, int n, vector<int> &precioNafta){
listaAdy res(61*n);
    for (int i = 0; i < 61*n; ++i) {
        for (int j = 0; j < 61*n; ++j) {
                if (nodoYnafta(i).first == nodoYnafta(j).first && i < j && nodoYnafta(i).second + 1 == nodoYnafta(j).second) {//cargo nafta
                    res[i].push_back(make_pair(j,precioNafta[nodoYnafta(i).first] * nodoYnafta(j).second - precioNafta[nodoYnafta(i).first]*nodoYnafta(i).second));
                }
                if (nodoYnafta(i).first != nodoYnafta(j).first && puedoIr(matAdy,precioNafta,i,j)){//me muevo a otra ciudad
                    res[i].push_back(make_pair(j,0));
                }
        }
    }
    return res;
}


resultadoOrdenado convertirSolucion(matriz &caminoMinimo, int n){
    resultadoOrdenado solucion;
    for (int k = 0; k < n; ++k) {
        for (int p = 0; p < n; ++p) {
            if (k < p) {
                int desde = k;
                int hasta = p;
                int costo = caminoMinimo[k*61][p*61];
                solucion[make_pair(desde, hasta)] = costo;
            }
        }
    }
    return solucion;
}


//Ejercicio 2 usando Dijkstra
resultadoOrdenado Ej2Dijkstra(matriz &matAdy, int n, vector<int> &precioNafta){
listaAdy res = generarLista(matAdy,n,precioNafta);
    matriz caminoMinimo;
    for (int i = 0; i < res.size(); ++i){
        caminoMinimo.push_back(Dijkstra(res, i+1));
    }
    resultadoOrdenado solucion = convertirSolucion(caminoMinimo,n);
    return solucion;
}

//Ejercicio 2 usando DijkstraPQ
resultadoOrdenado Ej2DijkstraPQ(matriz &matAdy, int n, vector<int> &precioNafta){
listaAdy res = generarLista(matAdy,n,precioNafta);
    matriz caminoMinimo;
    for (int i = 0; i < res.size(); ++i){
        caminoMinimo.push_back(DijkstraPQ(res, i+1));
    }
    resultadoOrdenado solucion = convertirSolucion(caminoMinimo,n);
    return solucion;
}


//Ejercicio 2 usando BellmanFord
resultadoOrdenado Ej2BellmanFord(matriz &matAdy, int n, vector<int> &precioNafta){
    listaAdy res = generarLista(matAdy,n,precioNafta);
    matriz caminoMinimo;
    for (int i = 0; i < res.size(); ++i){
        caminoMinimo.push_back(BellmanFord(res, i+1, res.size()));
    }
    resultadoOrdenado solucion = convertirSolucion(caminoMinimo,n);
    return solucion;
}


//Ejercicio 2 usando Floyd
resultadoOrdenado Ej2Floyd(matriz &matAdy, int n, vector<int> &precioNafta){
    matriz res = generarMatriz(matAdy,n,precioNafta);
    matriz caminoMinimo = Floyd_Warshall(res);
    resultadoOrdenado solucion = convertirSolucion(caminoMinimo,n);
    return solucion;
}


//Ejercicio 2 usando Dantzig
resultadoOrdenado Ej2Dantzig(matriz &matAdy, int n, vector<int> &precioNafta){
    matriz res = generarMatriz(matAdy,n,precioNafta);
    matriz caminoMinimo = dantzig(res);
    resultadoOrdenado solucion = convertirSolucion(caminoMinimo,n);
    return solucion;
}

//Ejercicio 2 usando A*
resultadoOrdenado Ej2A_estrella(listaAdy &pesos, int n){
    resultadoOrdenado solucion;
    for(int i = 0; i < n; i++){
    	for(int j = i + 1; j < n; j++){
    		if(i != j){
    			int camino_min = A_estrella(pesos,i+1,j+1);
    			pair<pair<int,int>,int> sol = make_pair(make_pair(i+1,j+1),camino_min);
    			solucion.insert(sol);
    		}
    	}
    }
    return solucion;
}



int main(int argc, char **argv){

	int n, m;

	cin >> n >> m;

	vector<int> precioNafta;                    //guardo los costos de la nafta en los nodos
	for (int i = 0; i < n; ++i)
	{
		int c;
		cin >> c;
		precioNafta.push_back(c);
	}


    matriz matAdy (n, vector<int> (n, inf));   // Iniciar matriz de n*n en inf

    Aristas ejes(m);

    for (int i = 0; i < n; ++i) {
        matAdy[i][i] = 0;                     //Pongo la diagonal en 0
    }

    for (int i = 0; i < m; ++i){
        int a, b, c;
        cin >> a >> b >> c;

        ejes[i]= make_pair(c, make_pair(a,b));
        matAdy[a-1][b-1] = c;
        matAdy[b-1][a-1] = c;
    }

//creo las listas de adyacencia para A* y dijkstra uno contra uno

    listaAdy listasAdy;
    for(int i = 0; i < n; i++){
   	    vector<pair<int,int> > vacio;
   	    listasAdy.push_back(vacio);
    }

    for(int i = 0; i < n; i++){
   	    for(int j = 0; j < n; j++){
   		   if(matAdy[i][j] != 0 and matAdy[i][j] != inf){
   			  listasAdy[i].push_back(make_pair(j,estimadora(i+1,j+1,n)));
   		   }
    	}
    }  

//  Parámetro de entrada argv[1]
//  1 Floyd_Warshall
//  2 Dantzig
//  3 Dijkstra
//  4 DijkstraPQ
//  5 Bellman-Ford
//  6 Compara A* y DijkstraPQ

//    cout << n << ',';     //cantidad de nodos
//    cout << m << ',';     //cantidad de ejes

    if (*argv[1] == '1'){

        //res Floyd_Warshall
        auto startFloyd = chrono::steady_clock::now();
        resultadoOrdenado ej2F = Ej2Floyd(matAdy,n,precioNafta);
        auto endFloyd = chrono::steady_clock::now();
        auto diffFloyd = endFloyd - startFloyd;

        // cout << chrono::duration <double, milli> (diffFloyd).count() << ',';                  //imprimo el tiempo de Floyd

        mostrarMap(ej2F, "Ej2 usando Floyd");
    
    }

    if (*argv[1] == '2'){

        //res Dantzig
        auto startDantzig = chrono::steady_clock::now();
        resultadoOrdenado ej2Dantz = Ej2Dantzig(matAdy,n,precioNafta);
        auto endDantzig = chrono::steady_clock::now();
        auto diffDantzig = endDantzig - startDantzig;

        // cout << chrono::duration <double, milli> (diffDantzig).count();                  //imprimo el tiempo de Dantzig

        mostrarMap(ej2Dantz, "Ej2 usando Dantzig");
    
    }

    if (*argv[1] == '3'){
        //res Dijkstra
        auto startDijkstra = chrono::steady_clock::now();
        resultadoOrdenado ej2Dij = Ej2Dijkstra(matAdy,n,precioNafta);
        auto endDijkstra = chrono::steady_clock::now();
        auto diffDijkstra = endDijkstra - startDijkstra;

        // cout << chrono::duration <double, milli> (diffDijkstra).count() << ',';                  //imprimo el tiempo de Dijkstra
   
        mostrarMap(ej2Dij, "Ej2 usando Dijkstra");
    }

        if (*argv[1] == '4'){

        //res DijkstraPQ
        auto startDijkstraPQ = chrono::steady_clock::now();
        resultadoOrdenado ej2DPQ = Ej2DijkstraPQ(matAdy,n,precioNafta);
        auto endDijkstraPQ = chrono::steady_clock::now();
        auto diffDijkstraPQ = endDijkstraPQ - startDijkstraPQ;

        // cout << chrono::duration <double, milli> (diffDijkstraPQ).count();                  //imprimo el tiempo de DijkstraPQ
   
        mostrarMap(ej2DPQ, "Ej2 usando DijkstraPQ");
    }

    if (*argv[1] == '5'){
        //res BellmanFord
        auto startBellmanFord = chrono::steady_clock::now();
        resultadoOrdenado ej2BF = Ej2BellmanFord(matAdy,n,precioNafta);
        auto endBellmanFord = chrono::steady_clock::now();
        auto diffBellmanFord = endBellmanFord - startBellmanFord;

        // cout << chrono::duration <double, milli> (diffBellmanFord).count();                  //imprimo el tiempo de BellmanFord
        
        mostrarMap(ej2BF, "Ej2 usando BellmanFord");

    }

    if(*argv[1] == '6'){
    	int mejores = 0;
    	int peores = 0;	
    	for(int i = 0; i < n; i++){
    		for(int j = 0; j < n; j++){

    			auto startA_estrella = chrono::steady_clock::now();
    			int Aestrella = A_estrella(listasAdy, i + 1 ,j + 1);
   				auto endA_estrella = chrono::steady_clock::now();
			    auto diffA_estrella = endA_estrella - startA_estrella;

			    auto startDijkstraPQunoAuno = chrono::steady_clock::now();
    			int DijPQunoAuno = DijkstraPQunoAuno(listasAdy, i + 1 , j + 1);
    			auto endDijkstraPQunoAuno = chrono::steady_clock::now();
			    auto diffDijkstraPQunoAuno = endDijkstraPQunoAuno - startDijkstraPQunoAuno;

			    if(diffDijkstraPQunoAuno <= diffA_estrella){
			    	peores ++;
			    }else{
			    	mejores++;
			    }
    		}
    	}
    	//cout << mejores << ',' << peores; 
    	resultadoOrdenado ej2A_s = Ej2A_estrella(listasAdy,n);
    	mostrarMap(ej2A_s, "Ej2 usando A*");
    }

    return 0;
}
