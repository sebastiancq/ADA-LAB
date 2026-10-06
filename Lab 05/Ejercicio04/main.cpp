#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <chrono>
#include <random>
#include <string>

using namespace std;
using namespace chrono;

struct Arista {
    int u, v, peso;
};

vector<int> padre, rango;

mt19937 generador(12345);


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
    int usadas = 0;

    for (auto& a : aristas) {

        if (unir(a.u, a.v)) {

            pesoTotal += a.peso;
            usadas++;

            if (usadas == n - 1)
                break;
        }
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


// Generar grafo aleatorio

vector<Arista> generarGrafo(
    int n,
    double densidad
) {

    vector<Arista> aristas;

    vector<vector<bool>> existe(
        n,
        vector<bool>(n, false)
    );

    uniform_int_distribution<int>
        pesoAleatorio(1, 100);

    uniform_int_distribution<int>
        verticeAleatorio(0, n - 1);


    // Crear una base conexa

    for (int i = 0; i < n - 1; i++) {

        int peso =
            pesoAleatorio(generador);

        aristas.push_back({
            i,
            i + 1,
            peso
        });

        existe[i][i + 1] = true;
        existe[i + 1][i] = true;
    }


    int maxAristas =
        n * (n - 1) / 2;

    int cantidadDeseada =
        static_cast<int>(
            maxAristas * densidad
        );

    if (cantidadDeseada < n - 1)
        cantidadDeseada = n - 1;


    while (
        static_cast<int>(aristas.size()) <
        cantidadDeseada
    ) {

        int u =
            verticeAleatorio(generador);

        int v =
            verticeAleatorio(generador);

        if (u == v)
            continue;

        if (existe[u][v])
            continue;

        int peso =
            pesoAleatorio(generador);

        aristas.push_back({
            u,
            v,
            peso
        });

        existe[u][v] = true;
        existe[v][u] = true;
    }


    return aristas;
}


// Crear lista de adyacencia

vector<vector<pair<int, int>>> crearGrafoPrim(
    int n,
    vector<Arista>& aristas
) {

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

    return grafo;
}


// Realizar prueba

void probar(
    int n,
    double densidad,
    string tipo
) {

    vector<Arista> aristas =
        generarGrafo(
            n,
            densidad
        );

    vector<vector<pair<int, int>>> grafo =
        crearGrafoPrim(
            n,
            aristas
        );


    auto inicioKruskal =
        high_resolution_clock::now();

    int pesoKruskal =
        kruskal(
            n,
            aristas
        );

    auto finKruskal =
        high_resolution_clock::now();


    auto inicioPrim =
        high_resolution_clock::now();

    int pesoPrim =
        prim(
            n,
            grafo
        );

    auto finPrim =
        high_resolution_clock::now();


    auto tiempoKruskal =
        duration_cast<microseconds>(
            finKruskal - inicioKruskal
        ).count();

    auto tiempoPrim =
        duration_cast<microseconds>(
            finPrim - inicioPrim
        ).count();


    cout << "\nTipo de grafo: "
         << tipo
         << endl;

    cout << "Vertices: "
         << n
         << endl;

    cout << "Aristas: "
         << aristas.size()
         << endl;

    cout << "Peso Kruskal: "
         << pesoKruskal
         << endl;

    cout << "Tiempo Kruskal: "
         << tiempoKruskal
         << " microsegundos"
         << endl;

    cout << "Peso Prim: "
         << pesoPrim
         << endl;

    cout << "Tiempo Prim: "
         << tiempoPrim
         << " microsegundos"
         << endl;
}


// Programa principal

int main() {

    vector<int> tamanios = {
        100,
        300,
        500
    };


    cout << "Grafos dispersos\n";

    for (int n : tamanios) {

        probar(
            n,
            0.02,
            "Disperso"
        );
    }


    cout << "\nGrafos densos\n";

    for (int n : tamanios) {

        probar(
            n,
            0.60,
            "Denso"
        );
    }


    return 0;
}