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
    vector<Arista> aristas
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

    for (auto& a : aristas) {

        if (unir(a.u, a.v))
            pesoTotal += a.peso;
    }

    return pesoTotal;
}


// Prim

int prim(
    int n,
    vector<vector<pair<int, int>>>& grafo
) {

    vector<bool> enArbol(n, false);

    priority_queue<
        pair<int, int>,
        vector<pair<int, int>>,
        greater<pair<int, int>>
    > pq;

    pq.push({0, 0});

    int pesoTotal = 0;

    while (!pq.empty()) {

        auto [peso, u] = pq.top();
        pq.pop();

        if (enArbol[u])
            continue;

        enArbol[u] = true;
        pesoTotal += peso;

        for (auto [v, p] : grafo[u]) {

            if (!enArbol[v])
                pq.push({p, v});
        }
    }

    return pesoTotal;
}


// Boruvka

int boruvka(
    int n,
    vector<Arista>& aristas,
    string nombres[]
) {

    padre.resize(n);
    rango.assign(n, 0);

    for (int i = 0; i < n; i++)
        padre[i] = i;

    int componentes = n;
    int pesoTotal = 0;
    int ronda = 1;

    while (componentes > 1) {

        vector<int> masBarata(n, -1);

        for (int i = 0; i < aristas.size(); i++) {

            int conjuntoU = encontrar(aristas[i].u);
            int conjuntoV = encontrar(aristas[i].v);

            if (conjuntoU == conjuntoV)
                continue;

            if (
                masBarata[conjuntoU] == -1 ||
                aristas[i].peso <
                aristas[masBarata[conjuntoU]].peso
            ) {
                masBarata[conjuntoU] = i;
            }

            if (
                masBarata[conjuntoV] == -1 ||
                aristas[i].peso <
                aristas[masBarata[conjuntoV]].peso
            ) {
                masBarata[conjuntoV] = i;
            }
        }


        cout << "\nRonda "
             << ronda
             << endl;

        bool seAgrego = false;


        for (int i = 0; i < n; i++) {

            int indice = masBarata[i];

            if (indice == -1)
                continue;

            Arista a = aristas[indice];

            if (unir(a.u, a.v)) {

                cout << nombres[a.u]
                     << " - "
                     << nombres[a.v]
                     << " peso "
                     << a.peso
                     << endl;

                pesoTotal += a.peso;
                componentes--;

                seAgrego = true;
            }
        }


        cout << "Componentes restantes: "
             << componentes
             << endl;


        if (!seAgrego) {

            cout << "El grafo no es conexo."
                 << endl;

            return -1;
        }

        ronda++;
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

        grafo[a.u].push_back({
            a.v,
            a.peso
        });

        grafo[a.v].push_back({
            a.u,
            a.peso
        });
    }


    int pesoKruskal =
        kruskal(n, aristas);

    int pesoPrim =
        prim(n, grafo);


    cout << "Boruvka\n";

    int pesoBoruvka =
        boruvka(
            n,
            aristas,
            nombres
        );


    cout << "\nComparacion final\n";

    cout << "Kruskal: "
         << pesoKruskal
         << endl;

    cout << "Prim: "
         << pesoPrim
         << endl;

    cout << "Boruvka: "
         << pesoBoruvka
         << endl;


    if (
        pesoKruskal == pesoPrim &&
        pesoPrim == pesoBoruvka
    ) {

        cout << "Los tres algoritmos obtuvieron "
             << "el mismo peso minimo."
             << endl;
    }
    else {

        cout << "Los resultados son diferentes."
             << endl;
    }


    return 0;
}