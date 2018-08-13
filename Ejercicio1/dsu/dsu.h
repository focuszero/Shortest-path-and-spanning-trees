#ifndef TP2_ALGO3_DSU_H
#define TP2_ALGO3_DSU_H
#include <vector>

using namespace std;

class dsu {


public:
        void init(int n);

        int find(int n);

        void unir(int n, int m);

private:
        vector<int> _componentes;

};


#endif //TP2_ALGO3_DSU_H
