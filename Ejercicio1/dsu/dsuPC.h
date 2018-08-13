#ifndef TP2_ALGO3_DSUPC_H
#define TP2_ALGO3_DSUPC_H
#include <vector>

using namespace std;

class dsuPC {


public:

        void initPC(int n);

        int findPC(int n);

        void unirPC(int n, int m);


private:
        vector<int> _altura;
        vector<int> _padres;

};


#endif //TP2_ALGO3_DSUPC_H
