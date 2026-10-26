#include <climits>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

size_t imprimirCaminos(int u, int destino,
                      const vector<vector<int>>& sucesoresOptimos,
                      const vector<string>& nombres, vector<int>& camino)
{
    camino.push_back(u);
    size_t cantidad = 0;

    if (u == destino) {
        for (size_t i = 0; i < camino.size(); ++i) {
            if (i > 0) cout << " -> ";
            cout << nombres[camino[i]];
        }
        cout << '\n';
        cantidad = 1;
    } else {
        for (int v : sucesoresOptimos[u]) {
            cantidad += imprimirCaminos(v, destino, sucesoresOptimos,
                                       nombres, camino);
        }
    }

    camino.pop_back();
    return cantidad;
}

int main()
{
    // Grafo del ejemplo docente: A; B, C, D; E, F, G; H.
    const int n = 8;
    const int destino = 7;
    vector<vector<pair<int, int>>> grafo(n);
    grafo[0] = {{1, 2}, {2, 4}, {3, 3}};
    grafo[1] = {{4, 7}, {5, 4}, {6, 6}};
    grafo[2] = {{4, 3}, {5, 2}, {6, 4}};
    grafo[3] = {{4, 4}, {5, 1}, {6, 5}};
    grafo[4] = {{7, 3}};
    grafo[5] = {{7, 6}};
    grafo[6] = {{7, 3}};

    vector<int> f(n, INT_MAX);
    vector<vector<int>> sucesoresOptimos(n);
    f[destino] = 0;

    // Todas las aristas van de un indice menor a uno mayor.
    // Por ello, G, F, E, D, C, B, A es un orden topologico inverso.
    for (int u = n - 2; u >= 0; --u) {
        for (const auto& arista : grafo[u]) {
            const int v = arista.first;
            const int costo = arista.second;
            if (f[v] == INT_MAX) continue;

            const int candidato = costo + f[v];
            if (candidato < f[u]) {
                f[u] = candidato;
                sucesoresOptimos[u].clear();
                sucesoresOptimos[u].push_back(v);
            } else if (candidato == f[u]) {
                sucesoresOptimos[u].push_back(v);
            }
        }
    }

    if (f[0] == INT_MAX) {
        cout << "No existe un camino desde A hasta H.\n";
        return 0;
    }

    const vector<string> nombres = {"A", "B", "C", "D", "E", "F", "G", "H"};
    vector<int> camino;
    cout << "Costo minimo desde A hasta H: " << f[0] << '\n';
    cout << "Caminos optimos:\n";
    const size_t cantidad = imprimirCaminos(0, destino, sucesoresOptimos,
                                          nombres, camino);
    cout << "Cantidad de caminos optimos: " << cantidad << '\n';
    return 0;
}
