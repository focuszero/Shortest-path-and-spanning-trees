#include "dsu.h"




inline void dsu::init(int n){
    for (int j = 0; j < n; ++j) {
        _componentes.push_back(j);
    }
}



inline int dsu::find(int n){
   return _componentes[n];
}


inline void dsu::unir(int n, int m){
int aux = _componentes[m];
    for (int i = 0; i < _componentes.size(); ++i)
    {
        if (_componentes[i] == aux)
        {
            _componentes[i] = _componentes[n];
        }
    }
}

