#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <string>

using namespace std;

struct Arista {
    int u, v, peso;
};

vector<int> padre, rango;


// Union Find

int encontrar(int x) {
    if (padre[x] != x)
        padre[x] = encontrar(padre[x]);

    return padre[x];
}

bool unir(int x, int y) {
    int rx = encontrar(x);
    int ry = encontrar(y);

    if (rx == ry)
        return false;

    if (rango[rx] < rango[ry])
        swap(rx, ry);

    padre[ry] = rx;

    if (rango[rx] == rango[ry])
        rango[rx]++;

    return true;
}


// Kruskal

int kruskal(
    int n,
    vector<Arista> aristas,
    string nombres[]
) {

    sort(
        aristas.begin(),
        aristas.end(),
        [](Arista a, Arista b) {
            return a.peso < b.peso;
        }
    );

    padre.resize(n);
    rango.assign(n, 0);

    for (int i = 0; i < n; i++)
        padre[i] = i;

    int pesoTotal = 0;

    cout << "Kruskal\n";

    for (auto& a : aristas) {

        if (unir(a.u, a.v)) {

            cout << nombres[a.u]
                 << " - "
                 << nombres[a.v]
                 << " peso "
                 << a.peso
                 << endl;

            pesoTotal += a.peso;
        }
    }

    return pesoTotal;
}


// Prim

int prim(
    int n,
    vector<vector<pair<int, int>>>& grafo,
    string nombres[]
) {

    vector<bool> enArbol(n, false);

    priority_queue<
        pair<int, pair<int, int>>,
        vector<pair<int, pair<int, int>>>,
        greater<pair<int, pair<int, int>>>
    > pq;

    pq.push({0, {0, -1}});

    int pesoTotal = 0;

    cout << "\nPrim\n";

    while (!pq.empty()) {

        auto actual = pq.top();
        pq.pop();

        int peso = actual.first;
        int u = actual.second.first;
        int origen = actual.second.second;

        if (enArbol[u])
            continue;

        enArbol[u] = true;
        pesoTotal += peso;

        if (origen != -1) {

            cout << nombres[origen]
                 << " - "
                 << nombres[u]
                 << " peso "
                 << peso
                 << endl;
        }

        for (auto [v, p] : grafo[u]) {

            if (!enArbol[v])
                pq.push({p, {v, u}});
        }
    }

    return pesoTotal;
}


// Programa principal

int main() {

    int n = 6;

    string nombres[] = {
        "A", "B", "C", "D", "E", "F"
    };

    vector<Arista> aristas = {
        {0, 1, 4},
        {0, 2, 2},
        {1, 2, 1},
        {1, 3, 5},
        {2, 3, 8},
        {2, 4, 10},
        {3, 4, 2},
        {3, 5, 6},
        {4, 5, 3}
    };


    vector<vector<pair<int, int>>> grafo(n);

    for (auto& a : aristas) {

        grafo[a.u].push_back({a.v, a.peso});
        grafo[a.v].push_back({a.u, a.peso});
    }


    int pesoKruskal =
        kruskal(n, aristas, nombres);

    int pesoPrim =
        prim(n, grafo, nombres);


    cout << "\nComparacion final\n";

    cout << "Peso Kruskal: "
         << pesoKruskal
         << endl;

    cout << "Peso Prim: "
         << pesoPrim
         << endl;


    if (pesoKruskal == pesoPrim) {

        cout << "Ambos algoritmos obtuvieron "
             << "el mismo peso minimo."
             << endl;
    }
    else {

        cout << "Los resultados son diferentes."
             << endl;
    }


    return 0;
}