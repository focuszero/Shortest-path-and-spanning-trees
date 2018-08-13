#include "dsuPC.h"


inline void dsuPC::initPC(int n){
    for (int j = 0; j < n; ++j) {
        _altura.push_back(1);
        _padres.push_back(j);
    }

}


inline int dsuPC::findPC(int n){
    if(_padres[n] != n){
        _padres[n] = findPC(_padres[n]);
    }
    return _padres[n];
}




inline void dsuPC::unirPC(int n, int m){
int x = findPC(n);
int y = findPC(m);
    if (_altura[x] < _altura[y]){
        _padres[x] = y;
    } else{
        _padres[y] = x;
    }
    if (_altura[x] == _altura[y]){
        _altura[x] = _altura[x] + 1;
    }
}
