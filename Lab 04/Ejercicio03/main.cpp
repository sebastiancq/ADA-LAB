#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <iomanip>
#include <algorithm>
#include <string>
using namespace std;

int main() {
    int n = 5;
    vector<string> ciudades = {"Lima", "Arequipa", "Cusco", "Puno", "Tacna"};

    vector<vector<int>> grafo(n, vector<int>(n, 0));

    auto agregarArista = [&](int u, int v, int peso) {
        grafo[u][v] = peso;
        grafo[v][u] = peso;
    };

    agregarArista(0, 1, 1015);
    agregarArista(0, 2, 1100);
    agregarArista(1, 2, 500);
    agregarArista(1, 3, 300);
    agregarArista(1, 4, 370);
    agregarArista(2, 3, 390);
    agregarArista(3, 4, 400);

    cout << "\nMatriz de adyacencia (distancia en km, 0 = sin conexion):\n";
    cout << setw(12) << "";
    for (const auto& ciudad : ciudades)
        cout << setw(10) << left << ciudad;
    cout << endl;

    for (int i = 0; i < n; i++) {
        cout << setw(12) << left << ciudades[i];
        for (int j = 0; j < n; j++)
            cout << setw(10) << grafo[i][j];
        cout << endl;
    }

    cout << "\nCiudades disponibles:\n";
    for (int i = 0; i < n; i++)
        cout << i << ": " << ciudades[i] << endl;

    int origen;
    cout << "\nIngrese el numero de origen: ";
    if (!(cin >> origen) || origen < 0 || origen >= n) {
        cout << "Indice invalido. Debe estar entre 0 y " << n - 1 << ".\n";
        return 1;
    }

    vector<int> dist(n, INT_MAX);
    dist[origen] = 0;

    vector<bool> visitado(n, false);
    vector<int> previo(n, -1);

    priority_queue<pair<int, int>,
                   vector<pair<int, int>>,
                   greater<pair<int, int>>> pq;
    pq.push({0, origen});

    // Se conserva el Dijkstra de tu ejercicio 01
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u]) continue;
        visitado[u] = true;

        for (int v = 0; v < n; v++) {
            if (!visitado[v] && grafo[u][v] != 0 &&
                dist[u] + grafo[u][v] < dist[v]) {

                dist[v] = dist[u] + grafo[u][v];
                previo[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    // CAMBIO: mostrar distancia y recorrido hacia cada destino
    cout << "\nRutas mas cortas desde " << ciudades[origen] << ":\n";

    for (int destino = 0; destino < n; destino++) {
        cout << "\nDestino: " << ciudades[destino] << '\n';

        if (dist[destino] == INT_MAX) {
            cout << "No existe ruta.\n";
            continue;
        }

        cout << "Distancia minima: " << dist[destino] << " km\n";

        vector<int> camino;
        for (int actual = destino; actual != -1; actual = previo[actual]) {
            camino.push_back(actual);
            if (actual == origen) break;
        }

        reverse(camino.begin(), camino.end());

        cout << "Recorrido: ";
        for (size_t i = 0; i < camino.size(); i++) {
            cout << ciudades[camino[i]];
            if (i + 1 < camino.size()) cout << " -> ";
        }
        cout << '\n';
    }

    return 0;
}