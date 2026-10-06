#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

const int INF = 1e9;

int main() {
    int n = 6; // A=0, B=1, C=2, D=3, E=4, F=5

    vector<vector<int>> dist(n, vector<int>(n, INF));
    vector<vector<int>> predecesor(n, vector<int>(n, -1));

    for (int i = 0; i < n; i++)
        dist[i][i] = 0;

    auto agregarArista = [&](int u, int v, int peso) {
        dist[u][v] = peso;
        dist[v][u] = peso;

        predecesor[u][v] = u;
        predecesor[v][u] = v;
    };

    // Mismas aristas del ejemplo resuelto en la guía
    agregarArista(0, 1, 4);
    agregarArista(0, 2, 2);
    agregarArista(1, 2, 1);
    agregarArista(1, 3, 5);
    agregarArista(2, 3, 8);
    agregarArista(2, 4, 10);
    agregarArista(3, 4, 2);
    agregarArista(3, 5, 6);
    agregarArista(4, 5, 3);

    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (dist[i][k] != INF && dist[k][j] != INF &&
                    dist[i][k] + dist[k][j] < dist[i][j]) {

                    dist[i][j] = dist[i][k] + dist[k][j];
                    predecesor[i][j] = predecesor[k][j];
                }

    string nombres[] = {"A", "B", "C", "D", "E", "F"};

    cout << "Matriz de distancias minimas entre todos los pares:\n\t";
    for (int j = 0; j < n; j++)
        cout << nombres[j] << "\t";
    cout << '\n';

    for (int i = 0; i < n; i++) {
        cout << nombres[i] << "\t";
        for (int j = 0; j < n; j++) {
            if (dist[i][j] == INF)
                cout << "INF\t";
            else
                cout << dist[i][j] << "\t";
        }
        cout << '\n';
    }

    int origen, destino;
    cout << "\nVertices: A=0, B=1, C=2, D=3, E=4, F=5\n";
    cout << "Ingrese origen y destino: ";
    cin >> origen >> destino;

    if (origen < 0 || origen >= n || destino < 0 || destino >= n) {
        cout << "Vertices invalidos.\n";
        return 1;
    }

    if (dist[origen][destino] == INF) {
        cout << "No existe camino entre esos vertices.\n";
        return 0;
    }

    // Reconstruir desde el destino hacia el origen
    vector<int> camino;
    int actual = destino;
    camino.push_back(actual);

    while (actual != origen) {
        actual = predecesor[origen][actual];
        camino.push_back(actual);
    }

    reverse(camino.begin(), camino.end());

    cout << "Distancia minima: " << dist[origen][destino] << '\n';
    cout << "Camino mas corto: ";

    for (size_t i = 0; i < camino.size(); i++) {
        if (i > 0) cout << " -> ";
        cout << nombres[camino[i]];
    }
    cout << '\n';

    return 0;
}