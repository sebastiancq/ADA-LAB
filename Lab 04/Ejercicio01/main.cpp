#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <iomanip>
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
    cout << '\n';

    for (int i = 0; i < n; i++) {
        cout << setw(12) << left << ciudades[i];

        for (int j = 0; j < n; j++)
            cout << setw(10) << grafo[i][j];

        cout << '\n';
    }

    cout << "\nCiudades disponibles:\n";
    for (int i = 0; i < n; i++)
        cout << i << ": " << ciudades[i] << '\n';

    int origen, destino;
    cout << "\nIngrese el numero de origen y destino: ";

    if (!(cin >> origen >> destino) ||
        origen < 0 || origen >= n ||
        destino < 0 || destino >= n) {
        cout << "Indices invalidos. Deben estar entre 0 y " << n - 1 << ".\n";
        return 1;
    }

    vector<int> dist(n, INT_MAX);
    vector<bool> visitado(n, false);
    dist[origen] = 0;

    priority_queue<pair<int, int>,
                   vector<pair<int, int>>,
                   greater<pair<int, int>>> pq;
    pq.push({0, origen});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u]) continue;
        visitado[u] = true;

        for (int v = 0; v < n; v++) {
            if (!visitado[v] && grafo[u][v] != 0 &&
                dist[u] + grafo[u][v] < dist[v]) {
                dist[v] = dist[u] + grafo[u][v];
                pq.push({dist[v], v});
            }
        }
    }

    if (dist[destino] == INT_MAX) {
        cout << "\nNo existe ruta entre " << ciudades[origen]
             << " y " << ciudades[destino] << ".\n";
    } else {
        cout << "\nDistancia mas corta de " << ciudades[origen]
             << " a " << ciudades[destino] << ": "
             << dist[destino] << " km\n";
    }

    return 0;
}